#include "utils.h"

string stripHttpHeaders(const string &text) {
  bool startsWithHttp = text.compare(0, 4, "HTTP") == 0;
  if (startsWithHttp) {
    size_t start = text.find("\n<", 0);
    if (start < text.length()) {
      return text.substr(start + 1);
    }
  }
  return text;
}

void lowerString(string &input) {
  for (size_t i = 0; i < input.length(); i++) {
    input[i] = tolower(input[i]);
  }
}

bool isAlphaNum(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

string stripSpecialChars(const string &input) {
  int start = 0;
  int end = input.length() - 1;
  for (; start <= end; start++) {
    if (isAlphaNum(input[start])) {
      break;
    }
  }
  for (; start < end; end--) {
    if (isAlphaNum(input[end])) {
      break;
    }
  }
  if (start < end) {
    return input.substr(start, end - start + 1);
  }

  return "";
}

int identifyCharset(const string &text) {
  size_t found = text.find("charset=");
  if (found < text.length()) {
    string charset = text.substr(found + 8, 3);
    lowerString(charset);
    if (charset == "iso") {
      return ISO_8859_1;
    } else if (charset == "utf") {
      return UTF_8;
    }
  }
  return UNKNOWN;  
}

/*
{
  char src[] = "";
  strcpy((char*) src, buf.c_str());

  char dst[2 * buf.length()];
  size_t srclen = buf.length();
  size_t dstlen = 2 * buf.length();

  fprintf(stderr, "in: %s\n", src);

  char * pIn = src;
  char * pOut = (char*) dst;

  iconv_t conv = iconv_open("UTF-8", "ISO-8859-1");
  iconv(conv, &pIn, &srclen, &pOut, &dstlen);
  iconv_close(conv);
  pOut[srclen] = '\0';

  fprintf(stderr, "out: %s\n\n", dst);
}
*/
