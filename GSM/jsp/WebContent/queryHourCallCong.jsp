<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="query.*,java.util.*"%>
<%	
	request.setCharacterEncoding("utf-8");
	String date = request.getParameter("date");
	int startTime = Integer.parseInt(request.getParameter("starttime"));
	int endTime = Integer.parseInt(request.getParameter("endtime"));
	ArrayList<Double> ls = new ArrayList<Double>();
	if (request.getParameter("date") != null){
		int DATE = QueryHourCallcong.dateToInt(request.getParameter("date"));
		int TIME_S = startTime * 10000;
		int TIME_E = endTime * 10000;
		int CELLID = Integer.parseInt(request.getParameter("cellid"));
		ls = QueryHourCallcong.select(DATE, TIME_S, TIME_E, CELLID);
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
		var lg = new html5jp.graph.line("CallCong");
		if (!lg) {
			return;
		}
		var items = [
			<%
				out.print("[" + '"' + "平均拥塞率" + '"');
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
			x : [ "时间"
			      <%
					for (int i = startTime; i < endTime; ++i){
						out.print(",");
						out.print('"');
						out.print(i + "点");
						out.print('"');
					}
				  %>
			],
			y : [ "平均拥塞率", 0.0, 0.2, 0.4, 0.6, 0.8, 1.0],
			yMax : 1.0,
			yMin : 0.0,
			lineWidth : [ 1, 2 ],
			dotRadius : [ 3, 4 ],
			dotType : [ "disc", "square" ]
		};
		lg.draw(items, params);
	};
</script>
<body>
	<center><h1><%out.print(date + "的小时级拥塞率"); %></h1></center>
	<div>
		<canvas width="1500" height="600" id="CallCong"></canvas>
	</div>
</body>
</html>