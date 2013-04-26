#ifndef __STRING_TOKENIZER_H__
#define __STRING_TOKENIZER_H__

#include <string>

using namespace std;

class StringTokenizer {
  public:
    virtual bool fetchChar(char &c) = 0;
    virtual bool isSeparator(const char &c) = 0;
    virtual bool fetch(string &term) = 0;
    virtual ~StringTokenizer() {}
};

StringTokenizer* stringTokenizer(const string* input, const int charset);

#endif
