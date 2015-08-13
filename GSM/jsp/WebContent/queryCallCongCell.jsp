<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="query.*,java.util.*"%>
<%
	request.setCharacterEncoding("utf-8");	
	String s = "";
	if (request.getParameter("date") != null){
		float congThreshold = Float.parseFloat(request.getParameter("congthreshold"))/100;
		int DATE = QueryCallCongCell.dateToInt(request.getParameter("date"));
		int TIME_S = Integer.parseInt(request.getParameter("starttime")) * 10000;
		int TIME_E = Integer.parseInt(request.getParameter("endtime")) * 10000;
		s = QueryCallCongCell.select(congThreshold, DATE, TIME_S, TIME_E);
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>拥塞小区查询</title>
</head>
<body>
	<table border="1">
		<tr>
			<td>小区ID</td>
			<td>时间</td>
			<td>小时级话务量</td>
			<td>每线话务量</td>
			<td>小时级拥塞率</td>
			<td>小时级半速率话务量比例</td>
		</tr>
		<%
			out.print(s);
		%>
	</table>
</body>
</html>