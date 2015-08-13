<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.sql.*,jxl.*,java.io.*,java.util.*,Table.Traffic_data" %>
<%@ page import="query.SQLServerInfo,query.FileUploadInfo" %>
<%
	request.setCharacterEncoding("utf-8");
	
	ArrayList<Traffic_data> arrRow = new ArrayList<Traffic_data>();
	try {		
		Workbook wb = null;
		Cell cell = null;
		InputStream stream = new FileInputStream(FileUploadInfo.getFileUploadDirectory() + "Traffic_data.xls");
		wb = Workbook.getWorkbook(stream);
		Sheet sheet = wb.getSheet(0);
		
		for (int i = 1; i < sheet.getRows(); ++i) {
			String[] str = new String[sheet.getColumns()];
			for (int j = 0; j < sheet.getColumns(); ++j) {
				cell = sheet.getCell(j, i);
				str[j] = cell.getContents();
			}
			Traffic_data row = new Traffic_data(str[0], str[1], str[2], str[3], str[4], str[5], 
					str[6], str[7], str[8], str[9]);
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

			String sql = "insert into Traffic_data values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
			PreparedStatement ps = conn.prepareStatement(sql);
			int batchSize = Integer.parseInt(request.getParameter("batchsize"));
			int count = 0;
			for (Traffic_data row : arrRow){
				ps.setInt(1, row.getT_DATE());
			    ps.setInt(2, row.getT_TIME());
			    ps.setInt(3, row.getCellID());
			    ps.setInt(4, row.getnTCH());
			    ps.setFloat(5, row.getThtraff());
			    ps.setFloat(6, row.getRate());
			    ps.setFloat(7, row.getThtraff());
			    ps.setInt(8, row.getCallnum());
			    ps.setInt(9, row.getCongsnum());
			    ps.setFloat(10, row.getCallcong());
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