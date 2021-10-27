#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager


#define BLYNK_PRINT Serial
#define SET_PIN 0





#define DHTPIN D3     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//auth token
char blynk_token[40] = "8oHHlrT1CY52XYwq5aqzaMDSNLmV8gyO";
char server[] = "blynk.honey.co.th";
int port = 8080;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


BlynkTimer timer;


//url ของ web server ที่รับข้อมูล
String url = "ggg";


float h, t;
//id ของอูปกรณ์เซ็นเซอร์
const char *deviceid = "dht22";

int Pump = D1;
int AirCold = D2;
unsigned long Dht22, last1;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.



BLYNK_WRITE(V4) // At global scope (not inside of the function)
{
    int i=param.asInt();
    if (i==1) 
    {
        digitalWrite(Pump, HIGH);
        Serial.println("asdawqeqwe");
    }
    else 
    {
        digitalWrite(Pump, LOW);
        Serial.println("1as5d6655wqe");
    }
}

  BLYNK_WRITE(V5) // At global scope (not inside of the function)
  {
     int i=param.asInt();
    if (i==1) 
    {
      
        digitalWrite(AirCold, HIGH);
         Serial.println("qwweq5dasade");
    }
    else 
    {
          digitalWrite(AirCold, LOW);
         Serial.println("y4sdaf564654");
    }
  }




void setup() {
  Serial.begin(115200);
  pinMode(Pump, OUTPUT);// กำหนดขาทำหน้าที่ให้ขา D1 เป็น OUTPUT
  pinMode(AirCold, OUTPUT);// กำหนดขาทำหน้าที่ให้ขา D2 เป็น OUTPUT
    Serial.println();
pinMode(SET_PIN, INPUT_PULLUP);
delay(3000);
  //*************************    การ อ่าน  เขียนค่า WiFi + Password ลงใน Node MCU ESP32   ************//

  //read configuration from FS json
  Serial.println("mounting FS...");//แสดงข้อความใน Serial Monitor

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");//แสดงข้อความใน Serial Monitor
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");//แสดงข้อความใน Serial Monitor
  }
  //end read
  Serial.println(blynk_token);

  //*************************   จบการ อ่าน  เขียนค่า WiFi + Password ]ลงใน Node MCU ESP32   **********//
  
  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);
WiFiManager wifiManager;
wifiManager.setSaveConfigCallback(saveConfigCallback);
wifiManager.addParameter(&custom_blynk_token);
if (digitalRead(SET_PIN) == LOW) {
wifiManager.resetSettings();
}
if (!wifiManager.autoConnect("Project Mushroom ")) {
Serial.println("failed to connect and hit timeout");
delay(3000);
ESP.reset();
delay(5000);
}
Serial.println("wifi connected");
strcpy(blynk_token, custom_blynk_token.getValue());
//save the custom parameters to FS
if (shouldSaveConfig) {
Serial.println("saving config");
DynamicJsonBuffer jsonBuffer;
JsonObject& json = jsonBuffer.createObject();
json["blynk_token"] = blynk_token;
File configFile = SPIFFS.open("/config.json", "w");
if (!configFile) { Serial.println("failed to open config file for writing");
}
json.printTo(Serial);
json.printTo(configFile);
configFile.close();
}
Serial.println();
Serial.print("local ip : ");
Serial.println(WiFi.localIP());
Serial.print("Blynk Token : ");
Serial.println(blynk_token);
Serial.print("id WIFI : ");
Serial.println(WiFi.SSID());
Blynk.config(blynk_token, server, port);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
     dht.begin();
  timer.setInterval(2000, SendSensor);
}

  void SendSensor()
{
  // Wait a few seconds between measurements.
  if (millis() - Dht22 >= 2000) {
    Dht22 = millis();
    // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);
  }
}

void loop() {

 Blynk.run();
 timer.run(); // run BlynkTimer
 if (millis() - last1 >= 3600000) {
    last1 = millis();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root ["deviceid"] = deviceid;
  root ["Humidity"] = h;
  root ["Temperature"] = t;
  String jsonStr;
  root.printTo(jsonStr);
  Serial.println(jsonStr);

  //ทำการส่ง request ไปที่ web server
  HTTPClient http;
  http.begin(url);
  //สร้าง Header เพื่อระบุชนิดข้อมูล ว่าที่ส่งไปเป็น json
  http.addHeader("Content-Type","application/json");
  Serial.println("[HTTP] POST...");

  //ส่ง request และ json (ส่งไปแบบ post) 
  int httpCode = http.POST(jsonStr);

  //ตรวจสอบ code ที่ response กลับมาและรายงานสถานะการส่ง
  if (httpCode > 0){
      Serial.printf("response code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.println(payload);
        }
       }
       else {
        Serial.print("[HTTP] failed "); Serial.print (httpCode);
        Serial.println(http.errorToString(httpCode).c_str());
        }
        http.end(); //สิ้นสุดการส่ง 
 }


}
