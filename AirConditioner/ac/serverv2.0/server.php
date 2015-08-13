<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-type" content="text/html; charset=UTF-8" />

<meta http-equiv="refresh" content="10" />

<title>server</title>
<meta http-equiv="Content-Language" content="en-us" />
<!-- Begin Stylesheets -->
<link rel="stylesheet" href="stylesheets/reset.css" type="text/css" media="screen" />
<link rel="stylesheet" href="stylesheets/server.css" type="text/css" media="screen" />
<!-- End Stylesheets -->
<!-- Begin JavaScript -->
<script type="text/javascript" src="javascripts/jquery-1.3.2.min.js"></script>
<script type="text/javascript" src="javascripts/jquery.easing.1.3.js"></script>
<script type="text/javascript" src="javascripts/jquery.server.js"></script>
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
    $arr=getdate();
    if ($arr["mon"] == 6 || $arr["mon"] == 7 || $arr["mon"] == 8){
        return 28;
    } else if ($arr["mon"] == 12 || $arr["mon"] == 1 || $arr["mon"] == 2){
        return 12;
    } else {
        return 18;
    }    
}





$servername = "localhost";
$username = "root";
$password = "iforgot";
$dbname = "ac";

//$conn=mysql_connect('localhost','root','');
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if (array_key_exists("switch", $_POST)){
    if ($_POST["switch"] == "开机"){
        $sql = "UPDATE server_status SET power = 'on' WHERE power is not null ";
        $conn->query($sql);
    	if(getdate()["mon"] >= 6 && getdate()["mon"] <= 11){
    		$sql = "UPDATE server_status SET cold_warm = '制冷' WHERE cold_warm is not null ";
            $conn->query($sql);
    	}
    	else{
    		$sql = "UPDATE server_status SET cold_warm = '制暖' WHERE cold_warm is not null ";
            $conn->query($sql);
    	}
    } else if ($_POST["switch"] == "关机"){
        $sql = "UPDATE server_status SET power = 'off' WHERE power is not null ";
        $conn->query($sql);
    }
}



/*if (array_key_exists("moshi", $_POST)){

    if ($_POST["moshi"] == "warm"){
	$sql = "UPDATE server_status SET cold_warm = '供暖' WHERE cold_warm is not null ";
        $conn->query($sql);
	$sql = "UPDATE client_status SET cold_warm = '供暖' WHERE cold_warm is not null ";
        $conn->query($sql);
    } else if ($_POST["moshi"] == "cold"){
        $sql = "UPDATE server_status SET cold_warm = '制冷' WHERE cold_warm is not null ";
        $conn->query($sql);
	$sql = "UPDATE client_status SET cold_warm = '制冷' WHERE cold_warm is not null ";
        $conn->query($sql);
    }
}*/




if(array_key_exists("mode", $_POST)){
	if($_POST["mode"]=="high"){
		$sql="update client_status set speed='高'";
		$conn->query($sql);
	}
	else if($_POST["mode"]=="mid"){
		$sql="update client_status set speed='中'";
		$conn->query($sql);
	}
	else if($_POST["mode"]=="low"){
		$sql="update client_status set speed='低'";
		$conn->query($sql);
	}
}


if(array_key_exists("set_temp", $_POST)){
		$temp11=$_POST["set_temp"];
		if((getInitRoomTempurature()==12)){
			if($temp11<25){
				$temp11=25;
			}
			else if($temp11>30){
				$temp11=30;
			}
			}
		else if((getInitRoomTempurature()==28)){
			if($temp11<18){
				$temp11=18;
			}
			else if($temp11>25){
				$temp11=25;
			}
			}
		else{
			if (getdate()["mon"] >= 9 && getdate()["mon"] <= 11){
                if($temp11<18){
                    $temp11=18;
                }
                else if($temp11>25){
                    $temp11=25;
                }
            } else {
                if($temp11<25){
                    $temp11=25;
                }
                else if($temp11>30){
                    $temp11=30;
                }
            }
		}
		$sql="update client_status set tempurature=$temp11";
		$conn->query($sql);
		$sql="update server_status set tempurature=$temp11";
		$conn->query($sql);
}


$sql = "SELECT * FROM server_status";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $server_status = $result->fetch_assoc();
}



$sql = "SELECT * FROM client_status";
$result1 = $conn->query($sql);
if ($result1->num_rows > 0) {
    
}



$sql = "SELECT * FROM client_start_time";
$result2 = $conn->query($sql);
if ($result2->num_rows > 0) {
    
}


?>



<div class="coda-slider-wrapper">
  <div class="coda-slider preload" id="coda-slider-1">
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">状态</h2>
        <center><form action="server.php" method="post">
            <input type="submit" name="switch" value="开机">
            <input type="submit" name="switch" value="关机"><br /><br />
        </form></center>
        <center>电源 <?php echo $server_status["power"];?></center>
        <center>设定温度 <?php 
		if(getInitRoomTempurature()==12){
			if($server_status["tempurature"]<25){
				echo 25;
			}
			else if($server_status["tempurature"]>30){
				echo 30;
			}
			else{
				echo $server_status["tempurature"];
				}
			}
		else if(getInitRoomTempurature()==28){
			if($server_status["tempurature"]<18){
				echo 18;
			}
			else if($server_status["tempurature"]>25){
				echo 25;
			}
			else{
				echo $server_status["tempurature"];
				}
			}
		else{
			echo $server_status["tempurature"];
			}
	
	
	

	?></center><br />
        <center>当前模式 <?php echo $server_status["cold_warm"];?></center><br />
      </div>
    </div>
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">设定</h2>
        
        
        <!--风速设定-->
        <form  action="server.php" method="post" onsubmit="return validateSubmit()">统一设定风速
            <input style="margin: 0 0 0 20px;" type="radio" name="mode" value="high"/>高风（1.3）
            <input style="margin: 0 0 0 20px;" type="radio" name="mode" value="mid"/>中风（1.0）
            <input style="margin: 0 0 0 20px;" type="radio" name="mode" value="low" />低风（0.8）
	    <input type="submit" value="确定" />
        </form>
        <!--风速设定over-->
        <!--温度设定-->


        <body>
            <p><form action="server.php" method="post" onsubmit="return validateSubmit()">统一设定温度
            <input id="p1" style="margin: 0 0 0 20px;" type="button" value="+" onClick = "change(1)">
		<input id="p2" type="text" value="25" name="set_temp">
            <input id="p1" style="margin: 0 0 0 20px;" type="button" value="-" onClick = "change(0)">
            <input type="submit" value="确定" />
            </form>
            </p><?php echo $server_status["power"]?>
	    <script type="text/javascript">
            function validateSubmit(){
                if ("<?php echo $server_status["power"]?>" == "off"){
                    alert("主机未开机！");
                    return false;
                }
            }

            function change(add){
                x=document.getElementById("p2");
                var temp= x.value;
		//document.write(temp);
		//document.write(parseFloat(temp));
                if (add == 1){

			//if((getInitRoomTempurature()==12)){
			//	if(temp!="30"){
			//	temp=parseFloat(temp) + 1;
			//}
			//}
		 //if((getInitRoomTempurature()==28)){
		//	if(temp!="25"){
			
				temp=parseFloat(temp) + 1;
		//	}
		//	}
		
                }
                if(add == 0){
                    //if((getInitRoomTempurature()==12)){
			//	if(temp!="25"){
				
			//	temp=parseFloat(temp) - 1;
			//}
			//}
		 //if((getInitRoomTempurature()==28)){
		//	if(temp!="18"){
			
				temp=parseFloat(temp) - 1;
		//	}
		//	}
                }
                document.getElementById("p2").value=temp;
            }
        </script>
        </body>
        <!--温度设定over-->
        
      </div>
    </div>

    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">监控</h2>
        <center><table>
            <tr>
                <td> 房间号 </td>
                <td> 电源 </td>
                <td> 设定温度 </td>
                <td> 房间温度 </td>
                <td> 风速 </td>
                <td> 费用 </td>
            </tr>
<?php 
    if ($server_status["power"] == "on"){
        $sql = "SELECT * FROM client_status";
        $result = $conn->query($sql);
        if ($result->num_rows > 0) {
            while ($client_status = $result->fetch_assoc()){
                if ($client_status["power"] == "on"){
                    $room_id = $client_status["room_id"];
                    $sql = "SELECT * FROM cost WHERE room_id = '$room_id' ";
                    $result2 = $conn->query($sql);
                    $standby = "(待机)";
                    if ($result2->num_rows > 0){
                        $standby = "";
                    }
                } else if ($client_status["power"] == "off"){
                    $standby = "";
                }
                    
                echo "<tr>";
                echo "<td>".$client_status["room_id"]."<td>".$client_status["power"].$standby."<td>".$client_status["tempurature"]."<td>".$client_status["room_tempurature"]."<td>".$client_status["speed"]."<td>".$client_status["total_cost"];
                echo "</tr><br>";
            }
        }
    }
?>     
        </table></center>
      </div>
    </div>
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">报表</h2>
        <form id="login-form" method="get" action="../statement/statement.php" target="_blank">
        <p style="margin: 0 0 0 80px;">起始时间<input type="date" id="startdate" name="startdate" required>
                <input style="margin: 0 0 0 30px;" type="submit" value="查询"></p>
        <p style="margin: 0 0 0 80px;">终止时间<input type="date" id="enddate" name="enddate" required></p>
        <br />
        </form>
      </div>
    </div>
  </div>
  <!-- .coda-slider -->
</div>
<!-- .coda-slider-wrapper -->


</body>
</html>