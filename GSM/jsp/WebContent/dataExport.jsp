<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="query.*,java.sql.*,java.util.*,java.io.*"%>
<%
	request.setCharacterEncoding("utf-8");
	String tableName = request.getParameter("tablename");

/* 	final String DB_DRIVER = SQLServerInfo.getDriver();
	final String DB_URL = SQLServerInfo.getURL();
	final String DB_USER = SQLServerInfo.getUserName();
	final String DB_PASSWORD = SQLServerInfo.getPasswd(); */
 	final String DB_DRIVER = "com.mysql.jdbc.Driver";
	final String DB_URL = "jdbc:mysql://localhost/GSM";
	final String DB_USER = "root";
	final String DB_PASSWORD = "iforgot";

	Connection conn = null;
	PreparedStatement prepstmt = null;
	ResultSet result = null;
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

		String sql = "select * from " + tableName;
		prepstmt = conn.prepareStatement(sql);
		result = prepstmt.executeQuery();

		ExportExcel.DB2Excel(result, tableName);

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
	
	// 文件
	File f = new File(DataExportInfo.getDataExportDirectory() + tableName + ".xls");
	
    response.setHeader("Content_Length", String.valueOf(f.length()));
	response.setContentType("application/vnd.ms-excel");
	response.setHeader(
			"Content-Disposition",
			"attachment;filename="
					+ new String(new String(tableName + ".xls")
							.getBytes("gb2312"), "iso8859-1"));

	FileInputStream in = new FileInputStream(f);
	OutputStream o = response.getOutputStream();
	int n = 0;
	byte[] b = new byte[1024];
	while ((n = in.read(b)) != -1) {
		o.write(b, 0, n);
	}
	in.close();
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>数据导出</title>
</head>
<body>

</body>
</html>