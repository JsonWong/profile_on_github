package query;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;

public class QueryCallCongCell {

	public static int dateToInt(String date) {
		char year = date.charAt(3);
		String month = date.substring(5, 7);
		String day = date.substring(8);
		String newDate = String.valueOf(year) + month + day;

		return Integer.parseInt(newDate);
	}

	public static String select(float Menx, int DATE, int TIME_S, int TIME_E) {

		Connection conn = null;

		final String DB_DRIVER = SQLServerInfo.getDriver();
		final String DB_URL = SQLServerInfo.getURL();
		final String DB_USER = SQLServerInfo.getUserName();
		final String DB_PASSWORD = SQLServerInfo.getPasswd();

		ResultSet rs = null;
		String returnStr = "";

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

			String sql = "{call Callcong_Cell(?,?,?,?)}";
			CallableStatement proc = conn.prepareCall(sql);
			proc.setFloat(1, Menx);
			proc.setInt(2, DATE);
			proc.setInt(3, TIME_S);
			proc.setInt(4, TIME_E);
			rs = proc.executeQuery();
			int columnCount = rs.getMetaData().getColumnCount();
			while (rs.next()) {
				returnStr += "<tr>";
				for (int i = 1; i <= columnCount; ++i) {
					returnStr += "<td>";
					returnStr += rs.getString(i);
					returnStr += "</td>";
				}
				returnStr += "</tr>";
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
