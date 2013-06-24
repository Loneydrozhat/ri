#include "string_tokenizer.h"
#include "utils.h"

class HtmlStringTokenizer : public StringTokenizer {
  public:
    HtmlStringTokenizer(const string* input) {
      input_ = input;
      pos_ = 0;
      len_ = input->length();
    }
    virtual bool fetch(string &term) {
      char c;
      bool termStarted = false;
      term.clear();
      while (fetchChar(c)) {
        if (isAlphaNum(c)) {
          term.push_back(c);
          termStarted = true;
        } else if (termStarted && isSeparator(c)) {
          break;
        }
      }
      return termStarted;
    }
    virtual bool fetchChar(char &c) {
      if (pos_ < len_) {
        unsigned char rc = input_->at(pos_);
        if (rc == '&') {
          c = decodeHtmlEntity(input_, pos_);
        } else if (rc >= 128) {
          c = decodeNonAsciiChar(input_, pos_);
        } else {
          c = tolower(rc);
        }
        pos_++;
        return true;
      }
      return false;
    }
    char decodeHtmlEntity(const string* input, unsigned int &pos) {
      string entity;
      for (pos++; pos < input->length(); pos++) {
        char c = input->at(pos);
        if (c == ';') {
          break;
        } else {
          entity.push_back(c);
        }
      }
      if (entity == "nbsp") {
        return ' ';
      }
      return '_';
    }
    virtual char decodeNonAsciiChar(const string* input, unsigned int &pos) = 0;
    virtual bool isAlphanum(const char &c) {
      return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
    }
    virtual bool isSeparator(const char &c) {
      switch (c) {
        case ' ':
        case '\n':
        case '\t':
        case '\r':
        case ':':
        case ';':
        case ',':
        case '.':
        case '/':
          return true;
        default:
          return false;
      }
    }
  private:
    const string* input_;
    unsigned int pos_;
    unsigned int len_;
};

class IsoHtmlStringTokenizer : public HtmlStringTokenizer {
  public:
    IsoHtmlStringTokenizer(const string* input)
    : HtmlStringTokenizer(input) {
    }
    virtual char decodeNonAsciiChar(const string* input, unsigned int &pos) {
      unsigned char raw = input->at(pos);
      switch(raw) {
        case 0xc0:
        case 0xe0:
        case 0xc1:
        case 0xe1:
        case 0xc2:
        case 0xe2:
        case 0xc3:
        case 0xe3:
          return 'a';
        case 0xc9:
        case 0xe9:
        case 0xca:
        case 0xea:
          return 'e';
        case 0xcd:
        case 0xed:
          return 'i';
        case 0xd3:
        case 0xf3:
        case 0xd4:
        case 0xf4:
        case 0xd5:
        case 0xf5:
          return 'o';
        case 0xda:
        case 0xfa:
          return 'u';
        case 0xc7:
        case 0xe7:
          return 'c';
        default:
          return '_';
      }
    }
};

class Utf8HtmlStringTokenizer : public HtmlStringTokenizer {
  public:
    Utf8HtmlStringTokenizer(const string* input)
    : HtmlStringTokenizer(input) {
    }
    virtual char decodeNonAsciiChar(const string* input, unsigned int &pos) {
      unsigned char first = input->at(pos);
      if (first == 0xc3 && (pos + 1) < input->length()) {
        unsigned char second = input->at(++pos);
        switch(second) {
          case 0x80:
          case 0xa0:
          case 0x81:
          case 0xa1:
          case 0x82:
          case 0xa2:
          case 0x83:
          case 0xa3:
            return 'a';
          case 0x89:
          case 0xa9:
          case 0x8a:
          case 0xaa:
            return 'e';
          case 0x8d:
          case 0xad:
            return 'i';
          case 0x93:
          case 0xb3:
          case 0x94:
          case 0xb4:
          case 0x95:
          case 0xb5:
            return 'o';
          case 0x9a:
          case 0xba:
            return 'u';
          case 0x87:
          case 0xa7:
            return 'c';
          default:
            return '_';
        }
      }
      return '_';
    }
};

StringTokenizer* stringTokenizer(const string* input, const int charset) {
  if (charset == UTF_8) {
    return new Utf8HtmlStringTokenizer(input);
  } else {
    return new IsoHtmlStringTokenizer(input);
  }
}
