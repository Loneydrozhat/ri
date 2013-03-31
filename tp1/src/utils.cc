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
