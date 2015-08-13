<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php 
session_start();
$room_id = $_GET["roomid"];
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-type" content="text/html; charset=UTF-8" />

<meta http-equiv="refresh" content="10;client.php?roomid=<?php echo $room_id;?>" />

<title>client</title>
<meta http-equiv="Content-Language" content="en-us" />
<meta name="author" content="Niall Doherty" />
<!-- Begin Stylesheets -->
<link rel="stylesheet" href="stylesheets/reset.css" type="text/css" media="screen" />
<link rel="stylesheet" href="stylesheets/client.css" type="text/css" media="screen" />
<!-- End Stylesheets -->
<!-- Begin JavaScript -->
<script type="text/javascript" src="javascripts/jquery-1.3.2.min.js"></script>
<script type="text/javascript" src="javascripts/jquery.easing.1.3.js"></script>
<script type="text/javascript" src="javascripts/jquery.client.js"></script>
<script type="text/javascript">
			$().ready(function() {
				$('#coda-slider-1').codaSlider();
			});
		 </script>
<!-- End JavaScript -->
</head>
<body class="coda-slider-no-js">
<noscript>
    
    <!-- ERROR -->
<div>
  <p>Unfortunately your browser does not hava JavaScript capabilities which are required to exploit full functionality of our site. This could be the result of two possible scenarios:</p>
  <ol>
    <li>You are using an old web browser, in which case you should upgrade it to a newer version. We recommend the latest version of <a href="http://www.getfirefox.com">Firefox</a>.</li>
    <li>You have disabled JavaScript in you browser, in which case you will have to enable it to properly use our site. <a href="http://www.google.com/support/bin/answer.py?answer=23852">Learn how to enable JavaScript</a>.</li>
  </ol>
</div>
</noscript>

<?php

function getInitRoomTempurature(){
    if (getdate()["mon"] == 6 || getdate()["mon"] == 7 || getdate()["mon"] == 8){
        return 28;
    } else if (getdate()["mon"] == 12 || getdate()["mon"] == 1 || getdate()["mon"] == 2){
        return 12;
    } else {
        return 18;
    }    
}

$servername = "localhost";
$username = "root";
$password = "iforgot";
$dbname = "ac";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT * FROM server_status";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $server_status = $result->fetch_assoc();
}

$sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $client_status = $result->fetch_assoc();
    $total_cost = $client_status["total_cost"];
}

$sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $cost = $result->fetch_assoc();
    if ($cost["speed"] == "高"){
        $total_cost += (time() - $cost["start_timestamp"])/60 * 1.3 * 5;
    } else if ($cost["speed"] == "中"){
        $total_cost += (time() - $cost["start_timestamp"])/60 * 1.0 * 5;
    } else if ($cost["speed"] == "低"){
        $total_cost += (time() - $cost["start_timestamp"])/60 * 0.8 * 5;
    }

    $sql = "UPDATE client_status SET total_cost = $total_cost WHERE room_id = '$room_id' ";
    $conn->query($sql);
    $new_timestamp = time();
    $sql = "UPDATE cost SET start_timestamp = $new_timestamp WHERE room_id = '$room_id' ";
    $conn->query($sql);
}    

if (array_key_exists("switch", $_POST)){
    if ($server_status["power"] == "on"){
        if ($_POST["switch"] == "开机"){
            if (!isset($_SESSION["is_switch_on_submitted"])){
                $_SESSION["is_switch_on_submitted"] = True;

                $sql = "UPDATE client_status SET power = 'on' WHERE room_id = '$room_id' ";
                $conn->query($sql);

                $sql = "UPDATE client_status SET tempurature =" . $server_status["tempurature"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);

                $sql = "UPDATE client_status SET speed = '中' WHERE room_id = '$room_id' ";
                $conn->query($sql);

                $sql = "UPDATE client_start_time SET start_year =" . getdate()["year"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);  
                $sql = "UPDATE client_start_time SET start_month =" . getdate()["mon"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);
                $sql = "UPDATE client_start_time SET start_day =" . getdate()["mday"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);
                $sql = "UPDATE client_start_time SET start_hour =" . getdate()["hours"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);
                $sql = "UPDATE client_start_time SET start_minute =" . getdate()["minutes"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);
                $sql = "UPDATE client_start_time SET start_second =" . getdate()["seconds"] . " WHERE room_id = '$room_id' ";
                $conn->query($sql);     
                $sql = "UPDATE client_start_time SET start_timestamp =" . time() . " WHERE room_id = '$room_id' ";
                $conn->query($sql);       

                $speed = $client_status["speed"];
                $sql = "INSERT INTO cost VALUES('$room_id', " . time() . ", '$speed')";
                $conn->query($sql);

                $room_tempurature = getInitRoomTempurature();
                $sql = "UPDATE client_status SET room_tempurature = $room_tempurature WHERE room_id = '$room_id' ";
                $conn->query($sql);

                $sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
                $result = $conn->query($sql);
                if ($result->num_rows > 0) {
                    $client_status = $result->fetch_assoc();
                }
                if ($client_status["tempurature"] != $client_status["room_tempurature"]){
                    $_SESSION["request_start_datetime"] = getdate()["year"]."-".getdate()["mon"]."-".getdate()["mday"]." ".getdate()["hours"].":".getdate()["minutes"].":".getdate()["seconds"];                    
                    $_SESSION["request_start_timestamp"] = getdate()[0];
                } else {
                    unset($_SESSION["request_start_datetime"]);
                    unset($_SESSION["request_start_timestamp"]);
                }

                $sql = "INSERT INTO statement_switch_on_off VALUES('$room_id', now(), '开机')";
                $conn->query($sql);
            }
        } else if ($_POST["switch"] == "关机"){
            if (isset($_SESSION["request_start_datetime"])){                
                $sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
                $result = $conn->query($sql);
                if ($result->num_rows > 0){
                    $cost = $result->fetch_assoc();
                    if ($cost["speed"] == "高"){
                        $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.3 * 5;
                    } else if ($cost["speed"] == "中"){
                        $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.0 * 5;
                    } else if ($cost["speed"] == "低"){
                        $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 0.8 * 5;
                    }
                    
                    $request_start_datetime = $_SESSION["request_start_datetime"];
                    $speed = $client_status["speed"];
                    $tempurature = $client_status["tempurature"];
                    $sql = "INSERT INTO statement_request_list VALUES('$room_id', '$request_start_datetime', now(), '$speed', '$tempurature', '$piriodical_cost')";
                    $conn->query($sql);
                }
            }

            $sql = "UPDATE client_status SET power = 'off' WHERE room_id = '$room_id' ";
            $conn->query($sql);

            $sql = "UPDATE client_status SET total_cost = 0 WHERE room_id = '$room_id' ";
            $conn->query($sql);

            $sql = "DELETE FROM cost WHERE room_id = '$room_id' ";
            $conn->query($sql);

            $sql = "INSERT INTO statement_switch_on_off VALUES('$room_id', now(), '关机')";
            $conn->query($sql);            

            session_destroy();
        }
    }
}

if (array_key_exists("speed", $_POST)){
    $sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
    $result = $conn->query($sql);
    if ($result->num_rows > 0) {
        $client_status = $result->fetch_assoc();
    }

    if (isset($_SESSION["request_start_datetime"])){

        $sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
        $result = $conn->query($sql);
        if ($result->num_rows > 0){
            $cost = $result->fetch_assoc();
            if ($cost["speed"] == "高"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.3 * 5;
            } else if ($cost["speed"] == "中"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.0 * 5;
            } else if ($cost["speed"] == "低"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 0.8 * 5;
            }
            
            $request_start_datetime = $_SESSION["request_start_datetime"];
            $speed = $client_status["speed"];
            $tempurature = $client_status["tempurature"];
            $sql = "INSERT INTO statement_request_list VALUES('$room_id', '$request_start_datetime', now(), '$speed', '$tempurature', '$piriodical_cost')";
            $conn->query($sql);

            $_SESSION["request_start_datetime"] = getdate()["year"]."-".getdate()["mon"]."-".getdate()["mday"]." ".getdate()["hours"].":".getdate()["minutes"].":".getdate()["seconds"];                    
            $_SESSION["request_start_timestamp"] = getdate()[0];
        }
    }

    $sql = "DELETE FROM cost where room_id = '$room_id' ";
    $conn->query($sql);

    if ($_POST["speed"] == "high"){
        $sql = "UPDATE client_status SET speed = '高' WHERE room_id = '$room_id' ";
        $conn->query($sql);

        $sql = "INSERT INTO cost VALUES('$room_id', " . time() . ", '高')";
        $conn->query($sql);
    } else if ($_POST["speed"] == "mid"){
        $sql = "UPDATE client_status SET speed = '中' WHERE room_id = '$room_id' ";
        $conn->query($sql);

        $sql = "INSERT INTO cost VALUES('$room_id', " . time() . ", '中')";
        $conn->query($sql);
    } else if ($_POST["speed"] == "low"){
        $sql = "UPDATE client_status SET speed = '低' WHERE room_id = '$room_id' ";
        $conn->query($sql);

        $sql = "INSERT INTO cost VALUES('$room_id', " . time() . ", '低')";
        $conn->query($sql);
    } 
}

if (array_key_exists("settemp", $_POST)){
    if (isset($_SESSION["request_start_datetime"])){
        $sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
        $result = $conn->query($sql);
        if ($result->num_rows > 0) {
            $client_status = $result->fetch_assoc();
        }

        $sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
        $result = $conn->query($sql);
        if ($result->num_rows > 0){
            $cost = $result->fetch_assoc();
            if ($cost["speed"] == "高"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.3 * 5;
            } else if ($cost["speed"] == "中"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.0 * 5;
            } else if ($cost["speed"] == "低"){
                $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 0.8 * 5;
            }
            
            $request_start_datetime = $_SESSION["request_start_datetime"];
            $speed = $client_status["speed"];
            $tempurature = $client_status["tempurature"];
            $sql = "INSERT INTO statement_request_list VALUES('$room_id', '$request_start_datetime', now(), '$speed', '$tempurature', '$piriodical_cost')";
            $conn->query($sql);

            $_SESSION["request_start_datetime"] = getdate()["year"]."-".getdate()["mon"]."-".getdate()["mday"]." ".getdate()["hours"].":".getdate()["minutes"].":".getdate()["seconds"];                    
            $_SESSION["request_start_timestamp"] = getdate()[0];
        }
    } else {
        $_SESSION["request_start_datetime"] = getdate()["year"]."-".getdate()["mon"]."-".getdate()["mday"]." ".getdate()["hours"].":".getdate()["minutes"].":".getdate()["seconds"];                    
        $_SESSION["request_start_timestamp"] = getdate()[0];        
    }

    if ($_POST["settemp"] == "+"){
        $sql = "UPDATE client_status SET tempurature = tempurature + 1 WHERE room_id = '$room_id' ";
        $conn->query($sql);
    } else if ($_POST["settemp"] == "-"){
        $sql = "UPDATE client_status SET tempurature = tempurature - 1 WHERE room_id = '$room_id' ";
        $conn->query($sql);
    }
}

if ($server_status["power"] == "off"){
    $sql = "UPDATE client_status SET power = 'off' WHERE room_id is not null";
    $conn->query($sql);
}

if ($server_status["cold_warm"] == "制冷"){
    $sql = "UPDATE client_status SET cold_warm = '制冷' WHERE room_id is not null";
    $conn->query($sql);
} else if ($server_status["cold_warm"] == "制暖"){
    $sql = "UPDATE client_status SET cold_warm = '制暖' WHERE room_id is not null";
    $conn->query($sql);
}

if ($client_status["power"] == "on"){
    $sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
    $result = $conn->query($sql);
    if ($result->num_rows > 0) {
        $client_status = $result->fetch_assoc();
    }

    if (!isset($_SESSION["tempurature_refresh_timestamp"])){
        $_SESSION["tempurature_refresh_timestamp"] = time();
    }
    if (isset($_SESSION["request_start_datetime"])){
        if ($client_status["speed"] == "中"){
            if (time() - $_SESSION["tempurature_refresh_timestamp"] >= 10){
                if ($client_status["tempurature"] > $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制暖"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature + 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                } else if ($client_status["tempurature"] < $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制冷"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature - 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                }              

                $_SESSION["tempurature_refresh_timestamp"] = time();
            }
        } else if ($client_status["speed"] == "高"){
            if (time() - $_SESSION["tempurature_refresh_timestamp"] >= 40){
                if ($client_status["tempurature"] > $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制暖"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature + 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                } else if ($client_status["tempurature"] < $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制冷"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature - 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                }

                $_SESSION["tempurature_refresh_timestamp"] = time();
            }
        } else if ($client_status["speed"] == "低"){
            if (time() - $_SESSION["tempurature_refresh_timestamp"] >= 80){
                if ($client_status["tempurature"] > $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制暖"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature + 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                } else if ($client_status["tempurature"] < $client_status["room_tempurature"]){
                    if ($client_status["cold_warm"] == "制冷"){
                        $sql = "UPDATE client_status SET room_tempurature = room_tempurature - 1 WHERE room_id = '$room_id' ";
                        $conn->query($sql);
                    }
                }

                $_SESSION["tempurature_refresh_timestamp"] = time();
            }
        }        
    }

    $sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
    $result = $conn->query($sql);
    if ($result->num_rows > 0) {
        $client_status = $result->fetch_assoc();
    }    

    if ($client_status["tempurature"] == $client_status["room_tempurature"]){
        if (isset($_SESSION["request_start_datetime"])){
            $sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
            $result = $conn->query($sql);
            if ($result->num_rows > 0){
                $cost = $result->fetch_assoc();
                if ($cost["speed"] == "高"){
                    $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.3 * 5;
                } else if ($cost["speed"] == "中"){
                    $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 1.0 * 5;
                } else if ($cost["speed"] == "低"){
                    $piriodical_cost = (time() - $_SESSION["request_start_timestamp"])/60 * 0.8 * 5;
                }
                
                $request_start_datetime = $_SESSION["request_start_datetime"];
                $speed = $client_status["speed"];
                $tempurature = $client_status["tempurature"];
                $sql = "INSERT INTO statement_request_list VALUES('$room_id', '$request_start_datetime', now(), '$speed', '$tempurature', '$piriodical_cost')";
                $conn->query($sql);
            }            

            unset($_SESSION["request_start_datetime"]);
            unset($_SESSION["request_start_timestamp"]);
        }

        $sql = "DELETE FROM cost WHERE room_id = '$room_id' ";
        $conn->query($sql);
    }

    if ($client_status["power"] == "on"){
        if (!isset($_SESSION["request_start_datetime"])){
            if (time() - $_SESSION["tempurature_refresh_timestamp"] >= 300){
                if ($client_status["room_tempurature"] >= getInitRoomTempurature()){
                    $sql = "UPDATE client_status SET room_tempurature = room_tempurature - 1 WHERE room_id = '$room_id' ";
                    $conn->query($sql);
                } else if ($client_status["room_tempurature"] < getInitRoomTempurature()){
                    $sql = "UPDATE client_status SET room_tempurature = room_tempurature + 1 WHERE room_id = '$room_id' ";
                    $conn->query($sql);
                }

                $new_timestamp = time();
                $speed = $client_status["speed"];
                $sql = "INSERT INTO cost VALUES('$room_id', '$new_timestamp', '$speed')";
                $conn->query($sql);

                $_SESSION["tempurature_refresh_timestamp"] = time();

                $_SESSION["request_start_datetime"] = getdate()["year"]."-".getdate()["mon"]."-".getdate()["mday"]." ".getdate()["hours"].":".getdate()["minutes"].":".getdate()["seconds"];
                $_SESSION["request_start_timestamp"] = getdate()[0];
            }
        } 
    }
}

$sql = "SELECT * FROM client_status WHERE room_id = '$room_id' ";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $client_status = $result->fetch_assoc();
}

$sql = "SELECT * FROM client_start_time WHERE room_id = '$room_id' ";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $client_start_time = $result->fetch_assoc();
}

$conn->close();

?>

<script type="text/javascript">

function validateSubmit(){
    if ("<?php echo $server_status["power"]?>" == "off"){
        alert("主机未开机！");
        return false;
    }
}

</script>

<div class="coda-slider-wrapper">
  <div class="coda-slider preload" id="coda-slider-1">
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">当前状态</h2>
        <center><!--img src="../images/thermometer.png" width="40px"--></center>
        <center>电源 <p><?php echo $client_status["power"];?><?php if ($client_status["power"] == "on" && !isset($_SESSION["request_start_datetime"])){ echo "(待机)";}?></p></center>
        <center>房间温度 <p id="roomtempurature"><?php if ($client_status["power"] == "on") echo $client_status["room_tempurature"]; else echo "--";?></p></center>
        <center>设定温度 <p id="tempurature"><?php if ($client_status["power"] == "on") echo $client_status["tempurature"]; else echo "--";?></p></center>
        <center><form action="client.php?roomid=<?php echo $client_status["room_id"]?>" method="post" onsubmit="return validateSubmit();">            
            <input type="submit" value="开机" name="switch">
            <input type="submit" value="关机" name="switch"><br />
        </form></center>
        <center>当前模式 <p id="mode"><?php if ($client_status["power"] == "on") echo $client_status["cold_warm"]; else echo "--";?></p></center>
        <center>当前风速 <p id="speed"><?php if ($client_status["power"] == "on") echo $client_status["speed"]; else echo "--";?></p></center><br />
      </div>
    </div>

    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">风速设定</h2>
        <!--风速设定-->        
        <center><form method="post" action="client.php?roomid=<?php echo $client_status["room_id"]?>" onsubmit="return validateSpeedSet();">
            <input type="radio" id="radio1" name="speed" value="high"/>高风（1.3）<br />
            <input type="radio" id="radio2" name="speed" value="mid"/>中风（1.0）<br />
            <input type="radio" id="radio3" name="speed" value="low" />低风（0.8）<br /><br />
            <input type="submit" value="确定" />
        </form></center>
        <script type="text/javascript">
            if ("<?php echo $client_status["speed"];?>" == "高"){
                document.getElementById("radio1").checked = true;
            } else if ("<?php echo $client_status["speed"];?>" == "中"){
                document.getElementById("radio2").checked = true;
            } else if ("<?php echo $client_status["speed"];?>" == "低"){
                document.getElementById("radio3").checked = true;
            }
                    
            function validateSpeedSet(){
                if (document.getElementById("p1").innerHTML == "--"){
                    alert("请先开机再调节风速！");
                    return false;
                }

                if (document.getElementById("speed").innerHTML == "--"){
                    alert("当前处于待机状态，不能调节风速！");
                    return false;
                }
            }
        </script>
        <!--风速设定over-->
      </div>
    </div>
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">温度设定</h2>
        <!--温度设定-->
        <center><p id="p1"><?php if ($client_status["power"] == "on") echo $client_status["tempurature"]; else echo "--";?></p></center><br />        
        <body>
        <script>
            function validateTempDown(){
                var mode = document.getElementById('mode').innerHTML;
                var temp = document.getElementById('tempurature').innerHTML;
                if (document.getElementById("p1").innerHTML == "--"){
                    alert("请先开机再调节温度！");
                    return false;
                }
                if (mode == "制冷"){
                    if (Number(temp) <= 18){
                        alert("超出调节范围！");
                        return false;
                    }
                } else if (mode == "制暖"){
                    if (Number(temp) <= 25){
                        alert("超出调节范围！");
                        return false;
                    }
                }
            }

            function validateTempUp(){
                var mode = document.getElementById('mode').innerHTML;
                var temp = document.getElementById('tempurature').innerHTML;
                if (document.getElementById("p1").innerHTML == "--"){
                    alert("请先开机再调节温度！");
                    return false;
                }
                if (mode == "制冷"){
                    if (Number(temp) >= 25){
                        alert("超出调节范围！");
                        return false;
                    }
                } else if (mode == "制暖"){
                    if (Number(temp) >= 30){
                        alert("超出调节范围！");
                        return false;
                    }
                }
            }
        </script>
            <center><form method="post" action="client.php?roomid=<?php echo $client_status["room_id"]?>#3" onsubmit="return validateTempUp();">
                <input type="submit" name="settemp" value="+">
            </form></center>
            <center><form method="post" action="client.php?roomid=<?php echo $client_status["room_id"]?>#3" onsubmit="return validateTempDown();">
                <input type="submit" name="settemp" value="-">
            </form></center>
        </body>
        <br />
        <!--温度设定over-->
      </div>
    </div>
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">费用</h2>
        <center><table>
            <tr>
                <td> 开机时间 </td>
                <td> &nbsp&nbsp使用时间 </td>
                <td> &nbsp&nbsp费用 </td>
            </tr>
            <tr>
                <td><?php if ($client_status["power"] == "on") echo $client_start_time["start_year"]."-".$client_start_time["start_month"]."-".$client_start_time["start_day"]." ".$client_start_time["start_hour"].":".$client_start_time["start_minute"].":".$client_start_time["start_second"]." "; else echo "--";?>&nbsp&nbsp&nbsp&nbsp</td>
                <td>&nbsp&nbsp<?php if ($client_status["power"] == "on") echo number_format((time() - $client_start_time["start_timestamp"]) / 60, 0); else echo "--";?></td>
                <td>&nbsp&nbsp<?php if ($client_status["power"] == "on") echo number_format($total_cost, 2); else echo "--";?></td>
            </tr>
        </table></center>
        <br />
        <p>费用说明：a) 每分钟中速风的能量消耗为一个标准功率消耗单位；
            b) 低速风的每分钟功率消耗为0.8标准功率；
            c) 高速风的每分钟功率消耗为1.3标准功率；
            每一个标准功率消耗的计费标准是5元</p>
      </div>
    </div>
  </div>
  <!-- .coda-slider -->
</div>
<!-- .coda-slider-wrapper -->
</body>
</html>