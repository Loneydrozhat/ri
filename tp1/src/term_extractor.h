#ifndef __TERM_EXTRACTOR_H__
#define __TERM_EXTRACTOR_H__

#include <string>

class TermExtractor {
  public:
    virtual bool hasNext() = 0;
    virtual std::string next() = 0;
    virtual ~TermExtractor() {
    }
};

TermExtractor* htmlExtractor(const std::string &html);

#endif