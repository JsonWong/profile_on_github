<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.sql.*,jxl.*,java.io.*,java.util.*,Table.MS" %>
<%@ page import="query.SQLServerInfo,query.FileUploadInfo" %>
<%
	request.setCharacterEncoding("utf-8");
	
	ArrayList<MS> arrRow = new ArrayList<MS>();
	try {		
		Workbook wb = null;
		Cell cell = null;
		InputStream stream = new FileInputStream(FileUploadInfo.getFileUploadDirectory() + "MS.xls");
		wb = Workbook.getWorkbook(stream);
		Sheet sheet = wb.getSheet(0);
		
		for (int i = 1; i < sheet.getRows(); ++i) {
			String[] str = new String[sheet.getColumns()];
			for (int j = 0; j < sheet.getColumns(); ++j) {
				cell = sheet.getCell(j, i);
				str[j] = cell.getContents();
			}
			MS row = new MS(str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);
			if (row.checkRow()) {
				arrRow.add(row);
			}
		}
	} catch (Exception e) {
		e.printStackTrace();
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>导入</title>
</head>
<body>
	<%
		final String DB_DRIVER = SQLServerInfo.getDriver();
		final String DB_URL = SQLServerInfo.getURL();
		final String DB_USER = SQLServerInfo.getUserName();
		final String DB_PASSWORD = SQLServerInfo.getPasswd();

		boolean success = false;
		Connection conn = null;
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

			String sql = "insert into MS values (?, ?, ?, ?, ?, ?, ?, ?)";
			PreparedStatement ps = conn.prepareStatement(sql);
			int batchSize = Integer.parseInt(request.getParameter("batchsize"));
			int count = 0;
			for (MS row : arrRow){
				ps.setString(1, row.getIMEI());
			    ps.setString(2, row.getMSISDN());
			    ps.setString(3, row.getUserName());
			    ps.setString(4, row.getMSCCompany());
			    ps.setInt(5, row.getGsmMspSense());
			    ps.setFloat(6, row.getGsmMsHeight());
			    ps.setFloat(7, row.getGsmMspFout());
			    ps.setString(8, row.getMZONE());
			    ps.addBatch();
			    if(++count % batchSize == 0) {
			        ps.executeBatch();
			    }
			}
			ps.executeBatch();
			ps.close();
			success = true;
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
<script>
	var success = <% out.print(success); %>;
	if (success == true){
		alert("导入成功！");
		window.close();
	} else{
		alert("导入失败！");
		window.close();
	}
</script>	
</body>
</html>