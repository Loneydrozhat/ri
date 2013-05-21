package utils;

import java.text.Normalizer;

public class StringUtils {

	public static String normalize(String input) {
		String output = Normalizer.normalize(input, Normalizer.Form.NFD);
		output = output.replaceAll("[^\\p{ASCII}]", "");
		return output.toLowerCase();
	}

}
