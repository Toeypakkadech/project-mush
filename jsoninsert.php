<?php
$json= file_get_contents('php://input');
$obi=json_decode($json);
$temp = $obi->{'t'}; //
$humid = $obi->{'h'}; //
$deviceid = $obi->{'deviceid'}; //
echo "$json \n";

$servername = "localhost"; // 
$username = "root";
$password = "";
$dbname = "jsondht22";

//
date_default_timezone_set('Asia/Bangkok');

$now = date('Y-m-d H:i:s'); //
echo $now; 

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}
echo "Connected successfully";

$conn->query("set NAMES utf8");//
$sql = "insert into jsonphpdht (deviceid, humid, temp, datetime)" ." VALUES ('$deviceid', '$humid', '$temp', '$now')";
if ($conn->query($sql)===TRUE){
echo ("บันทึกลงฐานข้อมูลเรียบร้อย");
}
else{
    echo "ERROR:" . $sql . "<br>" . $conn->error;
}
$conn->close();
?>