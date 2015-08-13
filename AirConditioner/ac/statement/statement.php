<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
</head>

<?php 

$start_date = $_GET["startdate"];
$end_date = $_GET["enddate"];

$servername = "localhost";
$username = "root";
$password = "iforgot";
$dbname = "ac";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT * FROM statement_request_list WHERE request_start_datetime >= date_format('$start_date', '%Y%m%d') and request_end_datetime <= date_format('$end_date', '%Y%m%d')";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    echo "<table>";
    echo "<tr>";
    echo "<td>房间号 <td>请求开始时间 <td>请求结束时间 <td>风速 <td>温度 <td>费用";
    echo "</tr>";
    while ($statement_request_list = $result->fetch_assoc()){
        echo "<tr>";
        echo "<td>".$statement_request_list["room_id"]."<td>".$statement_request_list["request_start_datetime"]."<td>".$statement_request_list["request_end_datetime"]."<td>".$statement_request_list["speed"]."<td>".$statement_request_list["tempurature"]."<td>".$statement_request_list["cost"];
        echo "</tr>";
    }
    echo "</table><br><br>";
}

$sql = "SELECT * FROM statement_switch_on_off WHERE time >= date_format('$start_date', '%Y%m%d') and time <= date_format('$end_date', '%Y%m%d')";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    echo "<table>";
    echo "<tr>";
    echo "<td>房间号 <td>开/关机时间 <td>开/关机";
    echo "</tr>";
    while ($statement_switch_on_off = $result->fetch_assoc()){
        echo "<tr>";
        echo "<td>".$statement_switch_on_off["room_id"]."<td>".$statement_switch_on_off["time"]."<td>".$statement_switch_on_off["behavior"];
        echo "</tr>";
    }
    echo "</table>";
}

$sql = "SELECT room_id, count(room_id) FROM statement_switch_on_off WHERE room_id IN(SELECT DISTINCT room_id FROM statement_switch_on_off) AND behavior = '开机' GROUP BY room_id";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    echo "<br>开机次数：<br>";
    while ($switch_on_count = $result->fetch_assoc()){
        echo $switch_on_count["room_id"].":&nbsp&nbsp".$switch_on_count["count(room_id)"];
        echo "<br>";
    }
}

$sql = "SELECT room_id, count(room_id) FROM statement_switch_on_off WHERE room_id IN(SELECT DISTINCT room_id FROM statement_switch_on_off) AND behavior = '关机' GROUP BY room_id";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    echo "<br>关机次数：<br>";
    while ($switch_off_count = $result->fetch_assoc()){
        echo $switch_off_count["room_id"].":&nbsp&nbsp".$switch_off_count["count(room_id)"];
        echo "<br>";
    }
}

?>

</html>
