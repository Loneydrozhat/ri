#include "utils.h"
#include "iconv.h"

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
