<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="query.SQLServerInfo" %>
<%
	request.setCharacterEncoding("utf-8");

	final String DB_DRIVER = SQLServerInfo.getDriver();
	final String DB_URL = SQLServerInfo.getURL();
	final String DB_USER = SQLServerInfo.getUserName();
	final String DB_PASSWORD = SQLServerInfo.getPasswd();

	Connection conn = null;
	ResultSet rs = null;
	String s = "";

	try {
		try {
			Class.forName(DB_DRIVER);
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		try {
			conn = DriverManager.getConnection(DB_URL, DB_USER,
					DB_PASSWORD);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		String sql = "{call Update_T()}";
		CallableStatement proc = conn.prepareCall(sql);
		rs = proc.executeQuery();
		int columnCount = rs.getMetaData().getColumnCount();
		while (rs.next()) {
			s += "<tr>";
			for (int i = 1; i <= columnCount; ++i) {
				s += "<td>";
				s += rs.getString(i);
				s += "</td>";
			}
			s += "</tr>";
		}
	} catch (Exception e) {
		e.printStackTrace();
	} finally {
		if (conn != null) {
			try {
				conn.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>查看更新后的数据</title>
</head>
<body>
	<table border="1">
		<tr>
			<td>每线话务量</td>
			<td>小时级话务量</td>
			<td>小时级拥塞数</td>
			<td>半速率话务量比例</td>
		</tr>
		<%
			out.print(s);
		%>
	</table>
</body>
</html>