import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.net.*;
import java.util.Hashtable;

public class dnsrelay {

	private static DatagramSocket socket = null;
	private DatagramPacket packet = null;
	private byte[] buf = null;
	private Hashtable<String, String> IPTable = null;

	public dnsrelay() throws Exception {
		socket = new DatagramSocket(53);
	}

	public void onReceivePacket() throws Exception {
		for (;;) {
			buf = new byte[512];
			packet = new DatagramPacket(buf, buf.length);
			socket.receive(packet);

			if ((buf[2] & 0x80) == 0x00) {
				QueryProcessor qp = new QueryProcessor(buf, packet, socket,
						IPTable);
				new Thread(qp).start();
			}
		}
	}

	public static void relayReturn(byte[] data, InetAddress addr, int port)
			throws Exception {
		DatagramPacket p = new DatagramPacket(data, data.length, addr, port);
		socket.send(p);
	}

	public void getIPTable() throws Exception {
		IPTable = new Hashtable<String, String>();

		String FileName = "dnsrelay.txt";
		BufferedReader in = new BufferedReader(new FileReader(
				new File(FileName)));

		String line = new String();
		line = in.readLine();
		while (line != null) {
			addToIPTable(line);
			line = in.readLine();
		}

		in.close();
	}

	void addToIPTable(String s) {
		int index = s.indexOf(' ');
		if (index > 0) {
			String ip = s.substring(0, index).trim();
			String DomainName = s.substring(index).trim();
			IPTable.put(DomainName, ip);
		}
	}

	public static void main(String[] args) throws Exception {
		dnsrelay instance = new dnsrelay();
		instance.getIPTable();
		instance.onReceivePacket();
	}

}
