<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="query.*,java.util.*"%>
<%	
	request.setCharacterEncoding("utf-8");
	String date = request.getParameter("date");
	int startTime = Integer.parseInt(request.getParameter("starttime"));
	int endTime = Integer.parseInt(request.getParameter("endtime"));
	int TIME_S = 0;
	int TIME_E = 0;
	int DATE = 0;
	int CELLID = 0;
	ArrayList<Double> ls = new ArrayList<Double>();
	if (request.getParameter("date") != null){
		DATE = Query15MinuteTraffic.dateToInt(request.getParameter("date"));
		TIME_S = startTime * 10000;
		TIME_E = endTime * 10000;
		CELLID = Integer.parseInt(request.getParameter("cellid"));
		ls = Query15MinuteTraffic.select(DATE, TIME_S, TIME_E, CELLID);
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>话务统计信息查询</title>
</head>
<script type="text/javascript" src="javascripts/line.js"></script>
<script type="text/javascript">
	window.onload = function() {
		var lg = new html5jp.graph.line("traff");
		if (!lg) {
			return;
		}
		var items = [
			<%
				out.print("[" + '"' + "平均话务量" + '"');
				if (!ls.isEmpty()){
					for (Iterator<Double> iter = ls.iterator(); iter.hasNext(); ){
						out.print(",");
						out.print(String.format("%.2f", iter.next()));
					}
				}
				out.print("]");
			%>
		];
		var params = {
			x : [ "分钟"
			      <%
					for (int i = TIME_S; i < TIME_E; i += 1500){
						if (i % 10000 < 6000){
							out.print(",");
							out.print('"');
							out.print(i/100);
							out.print('"');
						}						
					}
				  %>
			],
			y : [ "平均话务量", 0, 20, 40, 60, 80, 100, 120 ],
			yMax : 120,
			yMin : 0,
			lineWidth : [ 1, 2 ],
			dotRadius : [ 3, 4 ],
			dotType : [ "disc", "square" ]
		};
		lg.draw(items, params);
	};
</script>
<body>
	<center><h1><%out.print(date + "的十五分钟级话务量"); %></h1></center>
	<div>
		<canvas width="<%out.print(1500 * (endTime - startTime)); %>" height="600" id="traff"></canvas>
	</div>
</body>
</html>