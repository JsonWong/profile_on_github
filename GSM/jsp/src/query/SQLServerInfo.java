package query;

public class SQLServerInfo {
	private static String driver = "com.microsoft.sqlserver.jdbc.SQLServerDriver";
	private static String URL = "jdbc:sqlserver://10.8.161.126:1433;DatabaseName=GSM";
//	private static String URL = "jdbc:sqlserver://10.213.19.172:1433;DatabaseName=GSM";
	private static String userName = "sa";
	private static String passwd = "cc";
	
	public static String getDriver() {
		return driver;
	}
	public static String getURL() {
		return URL;
	}
	public static String getUserName() {
		return userName;
	}
	public static String getPasswd() {
		return passwd;
	}
}