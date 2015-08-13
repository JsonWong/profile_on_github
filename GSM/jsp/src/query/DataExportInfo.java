package query;

public class DataExportInfo {
	private static String dataExportDirectory = "";

	public static String getDataExportDirectory() {
		return dataExportDirectory;
	}

	public static void setDataExportDirectory(String dataExportDirectory) {
		DataExportInfo.dataExportDirectory = dataExportDirectory;
	}
}
