//#include <codecvt>
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


/*
// convert UTF-8 string to wstring
wstring utf8_to_wstring (const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
string wstring_to_utf8 (const wstring& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}
*/
