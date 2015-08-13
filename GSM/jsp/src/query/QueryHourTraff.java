package query;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.*;

public class QueryHourTraff {

	public static int dateToInt(String date) {
		char year = date.charAt(3);
		String month = date.substring(5, 7);
		String day = date.substring(8);
		String newDate = String.valueOf(year) + month + day;

		return Integer.parseInt(newDate);
	}

	public static ArrayList<Double> select(int DATE, int TIME_S, int TIME_E,
			int CELLID) {

		Connection conn = null;

		final String DB_DRIVER = SQLServerInfo.getDriver();
		final String DB_URL = SQLServerInfo.getURL();
		final String DB_USER = SQLServerInfo.getUserName();
		final String DB_PASSWORD = SQLServerInfo.getPasswd();

		ArrayList<Double> ls = new ArrayList<Double>();
		ResultSet rs = null;

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

			String sql = "{call Thour_traff(?,?,?,?)}";
			CallableStatement proc = conn.prepareCall(sql);
			proc.setInt(1, DATE);
			proc.setInt(2, TIME_S);
			proc.setInt(3, TIME_E);
			proc.setInt(4, CELLID);
			rs = proc.executeQuery();
			while (rs.next()){
				ls.add(rs.getDouble(1));
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
		return ls;

	}
}
