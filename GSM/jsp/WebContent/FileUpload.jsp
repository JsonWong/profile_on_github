<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*, javax.servlet.*"%>
<%@ page import="javax.servlet.http.*"%>
<%@ page import="org.apache.commons.fileupload.*"%>
<%@ page import="org.apache.commons.fileupload.disk.*"%>
<%@ page import="org.apache.commons.fileupload.servlet.*"%>
<%@ page import="org.apache.commons.io.output.*"%>
<%
	request.setCharacterEncoding("utf-8");
%>
<%
	File file;
	int maxFileSize = 50000 * 1024;
	int maxMemSize = 50000 * 1024;
	ServletContext context = pageContext.getServletContext();
	String filePath = context.getInitParameter("file-upload");

	// 验证上传内容了类型
	String contentType = request.getContentType();
	if ((contentType.indexOf("multipart/form-data") >= 0)) {

		DiskFileItemFactory factory = new DiskFileItemFactory();
		// 设置内存中存储文件的最大值
		factory.setSizeThreshold(maxMemSize);
		// 本地存储的数据大于 maxMemSize.
		factory.setRepository(new File("c:\\temp"));

		// 创建一个新的文件上传处理程序
		ServletFileUpload upload = new ServletFileUpload(factory);
		// 设置最大上传的文件大小
		upload.setSizeMax(maxFileSize);
		try {
	// 解析获取的文件
	List fileItems = upload.parseRequest(request);

	// 处理上传的文件
	Iterator i = fileItems.iterator();
	while (i.hasNext()) {
		FileItem fi = (FileItem) i.next();
		if (!fi.isFormField()) {
	// 获取上传文件的参数
	String fieldName = fi.getFieldName();
	String fileName = fi.getName();
	boolean isInMemory = fi.isInMemory();
	long sizeInBytes = fi.getSize();
	// 写入文件
	if (fileName.lastIndexOf("\\") >= 0) {
		file = new File(filePath,
		fileName.substring(fileName
				.lastIndexOf("\\")));
	} else {
		file = new File(filePath,
		fileName.substring(fileName
				.lastIndexOf("\\") + 1));
	}
	fi.write(file);
		}
	}
		} catch (Exception ex) {
	System.out.println(ex);
		}
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>选择要导入数据的表</title>
</head>
<body>
	<script>
		function checkForm(){
			if (document.getElementById("tablename").selectedIndex == 0){
				form.action = "importAdjcell.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 1){
				form.action = "importAnrenna.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 2){
				form.action = "importBSC.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 3){
				form.action = "importBTS.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 4){
				form.action = "importCell.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 5){
				form.action = "importCell_TCH.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 6){
				form.action = "importMS.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 7){
				form.action = "importMSC.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 8){
				form.action = "importRoad_data.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 9){
				form.action = "importTraffic_data.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 10){
				form.action = "importBSC_BTS.jsp";
			} else if (document.getElementById("tablename").selectedIndex == 11){
				form.action = "importMS_Cell_free.jsp";
			}
		}
	</script>
	<form name="form" method="post" onsubmit="return checkForm();">
		请选择要导入数据的表:
        		<select id="tablename">
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
        		批量导入数：
        		<select name="batchsize">
        			<option value="50">50</option>
        			<option value="100">100</option>
        			<option value="150">150</option>
        			<option value="200">200</option>
        			<option value="250">250</option>
        			<option value="300">300</option>
        			<option value="350">350</option>
        			<option value="400">400</option>
        			<option value="450">450</option>
        			<option value="500">500</option>
        		</select><br>
        		<input type="submit" value="确定">
	</form>
</body>
</html>