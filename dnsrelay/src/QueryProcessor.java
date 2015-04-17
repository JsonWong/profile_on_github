import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Hashtable;

public class QueryProcessor implements Runnable {

	private byte[] buf = null;
	private DatagramPacket packet = null;
	private DatagramSocket socket = null;
	private Hashtable<String, String> IPTable = null;
	private InetAddress DNSServerAddr = null;

	public QueryProcessor() {
	}

	public QueryProcessor(byte[] data, DatagramPacket p, DatagramSocket s,
			Hashtable<String, String> t) throws Exception {
		this.buf = data;
		this.packet = p;
		this.socket = s;
		this.IPTable = t;
		this.DNSServerAddr = InetAddress
				.getByAddress(new byte[] { 10, 3, 9, 4 });
	}

	String getDomainName(byte[] buf) {
		ArrayList<Integer> PointPos = new ArrayList<Integer>();
		int len = 0, times = 0;
		for (int i = 12; buf[i] != 0x00; ++i) {
			PointPos.add(Byte.toUnsignedInt(buf[i]));
			i += Byte.toUnsignedInt(buf[i]);
			++times;
		}
		for (int i = 0; i < PointPos.size(); ++i) {
			len += (int) PointPos.get(i);
		}

		String data = new String(buf, 0, buf.length);
		String question = data.substring(13, 12 + len + times);

		int temp = 0;
		StringBuffer strb = new StringBuffer(question);
		for (int i = 0; i < PointPos.size() - 1; ++i) {
			strb.setCharAt(temp + (int) PointPos.get(i), '.');
			question = strb.toString();
			temp += (int) PointPos.get(i) + 1;
		}

		return question;
	}

	byte[] convertIPAddrToBytes(String ipaddr) {
		byte[] IPBytes = new byte[4];
		int pos1 = ipaddr.indexOf('.');
		int pos2 = ipaddr.indexOf('.', pos1 + 1);
		int pos3 = ipaddr.indexOf('.', pos2 + 1);

		IPBytes[0] = (byte) Integer.parseInt(ipaddr.substring(0, pos1));
		IPBytes[1] = (byte) Integer.parseInt(ipaddr.substring(pos1 + 1, pos2));
		IPBytes[2] = (byte) Integer.parseInt(ipaddr.substring(pos2 + 1, pos3));
		IPBytes[3] = (byte) Integer.parseInt(ipaddr.substring(pos3 + 1));

		return IPBytes;
	}

	byte[] arrayCat(byte[] buf1, byte[] buf2) {
		byte[] bufret = null;
		int len1 = 0;
		int len2 = 0;
		if (buf1 != null)
			len1 = buf1.length;
		if (buf2 != null)
			len2 = buf2.length;
		if (len1 + len2 > 0)
			bufret = new byte[len1 + len2];
		if (len1 > 0)
			System.arraycopy(buf1, 0, bufret, 0, len1);
		if (len2 > 0)
			System.arraycopy(buf2, 0, bufret, len1, len2);
		return bufret;
	}

	void packetHandler() throws Exception {
		if (IPTable.containsKey(getDomainName(buf))) {
			if (IPTable.get(getDomainName(buf)).equals("0.0.0.0")) {
				// 返回"地址不存在"错误
				buf[2] = (byte) 0x81;
				buf[3] = 0x03;
				DatagramPacket PacketAnswer = new DatagramPacket(buf,
						buf.length, packet.getAddress(), packet.getPort());
				socket.send(PacketAnswer);
			} else {
				// 返回常规应答报文
				buf[2] = (byte) 0x81;
				buf[3] = 0x00;
				buf[7] = 0x01;
				byte[] temp = new byte[] { (byte) 0xc0, 0x0c, 0x00, 0x01, 0x00,
						0x01, 0x00, 0x00, 0x00, (byte) 0xff, 0x04 };
				byte[] ipaddr = convertIPAddrToBytes(IPTable
						.get(getDomainName(buf)));
				byte[] answer = arrayCat(temp, ipaddr);
				byte[] BufRet = buf;
				System.arraycopy(answer, 0, BufRet,
						getDomainName(buf).length() + 18, answer.length);

				DatagramPacket PacketAnswer = new DatagramPacket(BufRet,
						BufRet.length, packet.getAddress(), packet.getPort());
				socket.send(PacketAnswer);
			}
		} else {
			// 将查询报文转换ID后转发给远程DNS服务器，并接受应答报文，传递给主线程进行转发
			InetAddress ClientAddr = packet.getAddress();
			int ClientPort = packet.getPort();

			buf[0] = (byte) (0xff - buf[0]);
			buf[1] = (byte) (0xff - buf[1]);
			packet.setData(buf);
			packet.setAddress(DNSServerAddr);
			DatagramSocket s = new DatagramSocket(packet.getPort());
			s.setSoTimeout(5000);
			packet.setPort(53);
			s.send(packet);

			byte[] RecvBuf = new byte[512];
			DatagramPacket RecvPacket = new DatagramPacket(RecvBuf,
					RecvBuf.length);
			try {
				s.receive(RecvPacket);
			} catch (SocketTimeoutException e) {
				buf[2] = (byte) 0x81;
				buf[3] = 0x03;
				dnsrelay.relayReturn(buf, ClientAddr, ClientPort);
				s.close();
				return;
			}
			s.close();

			RecvBuf[0] = (byte) (0xff - RecvBuf[0]);
			RecvBuf[1] = (byte) (0xff - RecvBuf[1]);
			dnsrelay.relayReturn(RecvBuf, ClientAddr, ClientPort);
		}
	}

	@Override
	public void run() {
		try {
			packetHandler();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
