package query;

public class FileUploadInfo {
	private static String fileUploadDirectory = "";

	public static String getFileUploadDirectory() {
		return fileUploadDirectory;
	}

	public static void setFileUploadDirectory(String fileUploadDirectory) {
		FileUploadInfo.fileUploadDirectory = fileUploadDirectory;
	}
}
