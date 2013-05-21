package utils;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileUtils {

	public static String readNullTerminatedString(InputStream is)
			throws IOException {
		StringBuilder sb = new StringBuilder();
		while (true) {
			char c = (char) is.read();
			if (c == '\0') {
				return sb.toString();
			}
			sb.append(c);
		}
	}

	public static long readBleLong(DataInputStream is, int bytes) throws IOException {
		byte[] buffer = new byte[bytes];
		is.readFully(buffer);
		long num = 0;
		for (int i = 0; i < bytes; i++) {
			byte b = buffer[i];
			if (b == 0x00) {
				break;
			}
			num += ((long) b) << (i * 8);
		}
		return num;
	}

	public static int readBleInt(DataInputStream is, int bytes) throws IOException {
		return (int) readBleLong(is, bytes);
	}

}