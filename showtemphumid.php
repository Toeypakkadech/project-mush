
<html>
<head>
<title>RecordHumidTemp</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
 <link rel="stylesheet" href="bootstrap/css/bootstrap-theme.min.css">
<style>
table, td {
    border: 2px solid black;
    text-align: center;
    margin-left: auto; 
    margin-right: auto;
 
}
th{
    background-color: #CCFFFF;
    border: 2px solid black;
}
li{
  display: inline-block;
  margin-right:2px;
}
ul {
        text-align:center;
}

</style>
</head>
<body>
<h1 align = "center"><b>เว็บเก็บค่าอุณหภูมิและค่าความชื้นทุก 1 ชั่วโมง</b></h1>

<?php
$servername = "localhost"; // 
$username = "root";
$password = "";
$dbname = "jsondht22";



// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
    }
    $perpage = 5;
    if (isset($_GET['page'])) {
    $page = $_GET['page'];
    } else {
    $page = 1;
    }
    
    $start = ($page - 1) * $perpage;

    $sql = "SELECT deviceid, humid, temp, datetime FROM jsonphpdht limit {$start} , {$perpage} ";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table><tr><th>Deviceid</th>
    <th>Humidity</th>
    <th>Temperature</th>
    <th>Datetime</th>
    </th></tr>";
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<tr><td>" . $row["deviceid"]. "</td>
        <td>" . $row["humid"]. " </td>
        <td>" . $row["temp"]. "</td>
        <td>".$row["datetime"]."</td>
        </tr>";
    }
    echo "</table><br>";
} else {
    echo "0 results";
}
$sql2 = "SELECT deviceid, humid, temp, datetime FROM jsonphpdht";
$query2 = mysqli_query($conn, $sql2);
$total_record = mysqli_num_rows($query2);
$total_page = ceil($total_record / $perpage);
$conn->close();
?>

<nav>
 <ul class="pagination">
 <li>
 <a href="showtemphumid.php?page=1" aria-label="Previous">
 <span aria-hidden="true">&laquo;</span>
 </a>
 </li>
 <?php for($i=1;$i<=$total_page;$i++){ ?>
 <li><a href="showtemphumid.php?page=<?php echo $i; ?>"><?php echo $i; ?></a></li>
 <?php } ?>
 <li>
 <a href="showtemphumid.php?page=<?php echo $total_page;?>" aria-label="Next">
 <span aria-hidden="true">&raquo;</span>
 </a>
 </li>
 </ul>
 </nav>
 
 <script src="bootstrap/js/bootstrap.min.js"></script>
<h1 align = "center">
<div name="airvisual_widget" key="6177c5794227105a64448e68"></div></h1>
<script type="text/javascript" src="http://cdn.airvisual.net/assets/widgets/widget_v3.0.js"></script>


</body>
</html>