package query;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class QueryCell {
	public static String select(String Key) {
		String returnStr = "<title>CELL查询</title>";

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

		ResultSet result = null;

		try {
			try {
				Class.forName(DB_DRIVER);
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			try {
				conn = DriverManager
						.getConnection(DB_URL, DB_USER, DB_PASSWORD);
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			String sql = "select * from Cell where CellID =" + Key;
			;

			prepstmt = conn.prepareStatement(sql);

			result = prepstmt.executeQuery();
			int columnCount = result.getMetaData().getColumnCount();

			returnStr = "<head><title>Cell配置查询</title></head>";
			String s = "border=" + '"' + '1' + '"';
			returnStr += "<table " + s + ">";

			returnStr += "<tr>";
			for (int i = 1; i <= columnCount; ++i) {
				returnStr += "<td>";
				returnStr += result.getMetaData().getColumnName(i);
				returnStr += "</td>";
			}
			returnStr += "</tr>";

			while (result.next()) {
				returnStr += "<tr>";
				for (int i = 1; i <= columnCount; ++i) {
					returnStr += "<td>";
					returnStr += result.getString(i);
					returnStr += "</td>";
				}
				returnStr += "<tr>";
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
		return returnStr;
	}

}
