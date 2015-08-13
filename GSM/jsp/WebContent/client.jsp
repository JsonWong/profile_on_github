<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="query.*" %>
<%
	FileUploadInfo.setFileUploadDirectory("/Users/mac/apache-tomcat-7.0.59/webapps/FileUpload/");
	DataExportInfo.setDataExportDirectory("/Users/mac/apache-tomcat-7.0.59/webapps/DataExport/");
	
	request.setCharacterEncoding("utf-8");
	Connection conn = null;
	PreparedStatement prepstmt = null;
 	final String DB_DRIVER = "com.mysql.jdbc.Driver";
	final String DB_URL = "jdbc:mysql://localhost/GSM";
	final String DB_USER = "root";
	final String DB_PASSWORD = "iforgot";
	// final String DB_DRIVER = SQLServerInfo.getDriver();
	// final String DB_URL = SQLServerInfo.getURL();
	// final String DB_USER = SQLServerInfo.getUserName();
	// final String DB_PASSWORD = SQLServerInfo.getPasswd();
	ResultSet resultBts = null;
	ResultSet resultCell = null;
	try{
		Class.forName(DB_DRIVER);
		conn = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
		String sql = "select BtsName from BTS";
		prepstmt = conn.prepareStatement(sql);
		resultBts = prepstmt.executeQuery();
		
		sql = "select CellId from CELL order by CellId asc";
		prepstmt = conn.prepareStatement(sql);
		resultCell = prepstmt.executeQuery();
	} catch(Exception e){
		e.printStackTrace();
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<!--[if IE]>
<script>
(function(){if(!/*@cc_on!@*/0)return;var e = "abbr,article,aside,audio,canvas,datalist,date,
	details,dialog,eventsource,figure,footer,header,hgroup,mark,menu,meter,nav,output,progress,
	section,time,video".split(','),i=e.length;while(i--){document.createElement(e[i])}})()
</script>
<![endif]-->
<meta http-equiv="Content-type" content="text/html; charset=UTF-8" />
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
<div class="coda-slider-wrapper">
  <div class="coda-slider preload" id="coda-slider-1">
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">基本信息</h2>
        <center>
        <p>BTS查询</p>
          <form action="query.QueryBtsServlet" method="post" target="_blank">BtsName:
          	<select name="btsname">
          		<%
          			if (resultBts != null){
          				String optionTagBts;          				
          				while (resultBts.next()){
          					optionTagBts = "<option value=" + '"' + resultBts.getString(1) + '"' + ">" + 
          							resultBts.getString(1) + "</option>";
          					out.print(optionTagBts);
          				}
          			}
          		%>
          	</select>
            <input type="submit" value="给我查！">
          </form>
          <br>
          
          <p>CELL查询</p>
          <form action="query.QueryCellServlet" method="post" target="_blank">CellID:    
            <select name="cellid">
          		<%
          			String optionTagCell = "";
          			if (resultCell != null){          				
          				while (resultCell.next()){
          					optionTagCell += "<option value=" + '"' + resultCell.getString(1) + '"' + ">" + 
          							resultCell.getString(1) + "</option>";
          				}
          				out.print(optionTagCell);
          			}
          		%>
          	</select>
            <input type="submit" value="给我查！">
          </form>
          <br>
        </center>                        
      </div>
    </div>

    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">统计信息</h2>
        <script>
        	function validateQuerySelect(){
        		if (document.forms["traff"]["date"].value == ""){
        			alert("请选择日期！");
        			return false
        		}
        		
        		var startTimeIndex = Number(document.getElementById("starttime").selectedIndex);
        		var endTimeIndex = Number(document.getElementById("endtime").selectedIndex);
        		if (startTimeIndex >= endTimeIndex){
        			alert("请选择正确的时间段！");
        			return false;
        		}
        		
        		var queryContentIndex = Number(document.getElementById("querycontent").selectedIndex);
        		if (queryContentIndex == 0){
        			traff.action = "queryHourTraffic.jsp";
        		} else if (queryContentIndex == 1){
        			traff.action = "queryHourCallCong.jsp";
        		} else if (queryContentIndex == 2){
        			traff.action = "queryHourThtraff.jsp";
        		}
        	}
        </script>
        <center>        
        	<h3>话务统计信息查询</h3><br>
        	<form name="traff" method="post" target="_blank" onsubmit="return validateQuerySelect();">
        		CellID:<select name="cellid">
        			<%
        			if (resultCell != null){
        				out.print(optionTagCell);
        			}
        			%>
	          	</select><br>
        		日期：<input type="date" name="date"><br>
        		开始时间:<select name="starttime" id="starttime">
        			<%
        				String timeSelection = "";
        				for(int i = 0; i <= 23; ++i){
        					timeSelection += "<option>" + i + "</option>";
        				}
        				out.print(timeSelection);
        			%>
        		</select>点&nbsp&nbsp&nbsp
        		结束时间:<select name="endtime" id="endtime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点<br>查询内容:
        		<select id="querycontent">
        			<option>小时级平均话务量</option>
        			<option>小时级平均拥塞率</option>
        			<option>小时级半速率话务比例</option>
        		</select><br>
        		<input type="submit" value="给我查！">
        	</form>
        </center>
      </div>
    </div>

    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">话务分析</h2>
        <script>
        	function validateHourAnalysisSelect(){
        		if (document.forms["houranalysis"]["date"].value == ""){
        			alert("请选择日期！");
        			return false
        		}
        		
        		var startTimeIndex = Number(document.getElementById("houranalysisstarttime").selectedIndex);
        		var endTimeIndex = Number(document.getElementById("houranalysisendtime").selectedIndex);
        		if (startTimeIndex >= endTimeIndex){
        			alert("请选择正确的时间段！");
        			return false;
        		}
        		
        		var queryContentIndex = Number(document.getElementById("houranalysiscontent").selectedIndex);
        		if (queryContentIndex == 0){
        			houranalysis.action = "queryHourTraffic.jsp";
        		} else if (queryContentIndex == 1){
        			houranalysis.action = "queryHourCallCong.jsp";
        		} else if (queryContentIndex == 2){
        			houranalysis.action = "queryHourNTCH.jsp";
        		}
        	}
        </script>
        <center>
        	<h3>小时级话务查询</h3><br>
        	<form name="houranalysis" method="post" target="_blank" onsubmit="return validateHourAnalysisSelect();">
        		CellID:<select name="cellid">
        			<%
        			if (resultCell != null){
        				out.print(optionTagCell);
        			}
        			%>
	          	</select><br>
        		日期：<input type="date" name="date"><br>
        		开始时间:<select name="starttime" id="houranalysisstarttime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点&nbsp&nbsp&nbsp
        		结束时间:<select name="endtime" id="houranalysisendtime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点<br>查询内容:
        		<select id="houranalysiscontent">
        			<option>小时级平均话务量</option>
        			<option>小时级平均拥塞率</option>
        			<option>小时级每线话务量</option>
        		</select><br>
        		<input type="submit" value="给我查！">
        	</form>
        </center>
        <br>
        
        <script>
        	function validateMinuteAnalysisSelect(){
        		if (document.forms["minuteanalysis"]["date"].value == ""){
        			alert("请选择日期！");
        			return false
        		}
        		
        		var startTimeIndex = Number(document.getElementById("minuteanalysisstarttime").selectedIndex);
        		var endTimeIndex = Number(document.getElementById("minuteanalysisendtime").selectedIndex);
        		if (startTimeIndex >= endTimeIndex){
        			alert("请选择正确的时间段！");
        			return false;
        		}
        		
        		var queryContentIndex = Number(document.getElementById("minuteanalysiscontent").selectedIndex);
        		var minuteIntervalIndex = Number(document.getElementById("minuteinterval").selectedIndex);
        		if (queryContentIndex == 0){
        			if (minuteIntervalIndex == 0){
        				minuteanalysis.action = "queryMinuteTraffic.jsp";
        			} else if (minuteIntervalIndex == 1){
        				minuteanalysis.action = "query15MinuteTraffic.jsp";
        			}					
        		} else if (queryContentIndex == 1){
					if (minuteIntervalIndex == 0){
						minuteanalysis.action = "queryMinuteCallCong.jsp";
        			} else if (minuteIntervalIndex == 1){
        				minuteanalysis.action = "query15MinuteCallCong.jsp";
        			}
        		} else if (queryContentIndex == 2){
					if (minuteIntervalIndex == 0){
						minuteanalysis.action = "queryMinuteNTCH.jsp";
        			} else if (minuteIntervalIndex == 1){
        				minuteanalysis.action = "query15MinuteNTCH.jsp";
        			}
        		}
        	}
        </script>
        <center>
        	<h3>分钟级话务查询</h3><br>
        	<form name="minuteanalysis" method="post" target="_blank" onsubmit="return validateMinuteAnalysisSelect();">
        		CellID:<select name="cellid">
        			<%
        			if (resultCell != null){
        				out.print(optionTagCell);
        			}
        			%>
	          	</select><br>
        		日期：<input type="date" name="date"><br>
        		开始时间:<select name="starttime" id="minuteanalysisstarttime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点&nbsp&nbsp&nbsp
        		结束时间:<select name="endtime" id="minuteanalysisendtime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点<br>查询内容:
        		<select id="minuteanalysiscontent">
        			<option>分钟级平均话务量</option>
        			<option>分钟级平均拥塞率</option>
        			<option>分钟级每线话务量</option>
        		</select><br>时间间隔:
        		<select id="minuteinterval">
        			<option>一分钟</option>
        			<option>十五分钟</option>
        		</select><br>
        		<input type="submit" value="给我查！">
        	</form>
        </center><br>
        
        <script>
        	function validateCongcellQuerySelect(){
        		if (document.forms["congcell"]["date"].value == ""){
        			alert("请选择日期！");
        			return false
        		}        		
        		
        		var startTimeIndex = Number(document.getElementById("congcellstarttime").selectedIndex);
        		var endTimeIndex = Number(document.getElementById("congcellendtime").selectedIndex);
        		if (startTimeIndex >= endTimeIndex){
        			alert("请选择正确的时间段！");
        			return false;
        		}
        		
        		congcell.action = "queryCallCongCell.jsp";
        	}
        </script>
        <center>
        	<h3>拥塞小区查询</h3><br>
        	<form name="congcell" method="post" target="_blank" onsubmit="return validateCongcellQuerySelect();">
        		<lable>拥塞门限(%):</lable>
        		<select name="congthreshold">
        			<option value="10">10</option>
        			<option value="20">20</option>
        			<option value="30">30</option>
        			<option value="40">40</option>
        			<option value="50">50</option>
        			<option value="60">60</option>
        			<option value="70">70</option>
        			<option value="80">80</option>
        			<option value="90">90</option>
        			<option value="100">100</option>
        		</select><br>
        		日期：<input type="date" name="date"><br>
        		开始时间:<select name="starttime" id="congcellstarttime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点&nbsp&nbsp&nbsp
        		结束时间:<select name="endtime" id="congcellendtime">
        			<%
        				out.print(timeSelection);
        			%>
        		</select>点<br>
        		<input type="submit" value="给我查！">
        	</form>
        </center><br>
        
        <center>        	        	
        	<form action="updateData.jsp" target="_blank" method="post">
        		<lable>查看更新后的数据：</lable>
        		<input type="submit" value="查看">
        	</form>
        </center>
      </div>
    </div>
    
    <script>
    	function setDistance(){
    		var index = document.getElementById("distance").selectedIndex;    		
    	}
    </script>
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">邻区信息</h2>
        <center>
        	<h3>邻区计算</h3><br>
        	<form method="post" target="_blank" action="neighborCellCalculation.jsp" onsubmit="setDistance();">
        		<lable>距离阀值(米):</lable>        		
        		<select id="distance" name="distance">
        			<option value="1000">1000</option>
        			<option value="2000">2000</option>
        			<option value="3000">3000</option>
        		</select><br>
        		<input type="submit" value="给我查！">
        	</form>
        </center><br>
        
        <center>
        	<h3>邻区查询</h3><br>
        	<form method="post" target="_blank" action="neighborCellQuery.jsp">
        		<lable>距离:</lable>        		
        		<select id="distance" name="distance">
        			<option value="1000">1000</option>
        			<option value="2000">2000</option>
        			<option value="3000">3000</option>
        		</select><br>
        		CellID:<select name="cellid" id="cellid">
        			<%
        				out.print(optionTagCell);
        			%>
        		</select><br>
        		<input type="submit" value="给我查！">
        	</form>
        </center>
      </div>
    </div>
    
    <div class="panel">
      <div class="panel-wrapper">
        <h2 class="title">数据管理</h2>
        <script>
        	function checkFileName(){
        		if (document.getElementById("file").value == ""){
        			alert("请选择文件！");
        			return false;
        		}
        	}
        </script>
        <center>
        	<h3>从Excel导入数据到数据库</h3><br>
        	<form action="FileUpload.jsp" method="post" enctype="multipart/form-data" target="_blank" onsubmit="return checkFileName();">        		        		
        		请选择Excel文件:
        		<input type="file" name="file" id="file"><br>
        		<input type="submit" value="确定">
        	</form>
        </center><br>
        
        <center>
        	<h3>将数据库数据导出</h3><br>
        	<form action="dataExport.jsp" method="post" target="_blank">
        		请选择要导出的表:
        		<select name="tablename">
        			<option value="Adjcell">Adjcell</option>
        			<option value="Anrenna">Anrenna</option>
        			<option value="BSC">BSC</option>
        			<option value="BTS">BTS</option>
        			<option value="Cell">Cell</option>
        			<option value="Cell_TCH">Cell_TCH</option>
        			<option value="MS">MS</option>
        			<option value="MSC">MSC</option>
        			<option value="Road_data">Road_data</option>
        			<option value="traffic_data">traffic_data</option>
        			<option value="BSC_BTS">BSC_BTS</option>
        			<option value="MS_Cell_free">MS_Cell_free</option>
        		</select><br>
        		<input type="submit" value="确定">
        	</form>
        </center>
      </div>
    </div>
  </div>
  <!-- .coda-slider -->
</div>
<!-- .coda-slider-wrapper -->
</body>
<%
	try{
		if (conn != null)
			conn.close();		
	} catch(Exception e){
		e.printStackTrace();
	}
%>
</html>