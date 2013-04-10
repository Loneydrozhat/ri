#ifndef __INVERTED_LIST_H__
#define __INVERTED_LIST_H__

#include <string>
#include "utils.h"
#include "vocabulary.h"

using namespace std;

class InvertedList {
  public:
    virtual ~InvertedList() {}
};

class InvertedListWriter {
  public:
    virtual ~InvertedListWriter() {}
    virtual void writeEntry(int_id term, int_id doc, int_id tf) {}
    virtual void close() {}
};

InvertedListWriter* createInvertedListWriter(Vocabulary &vocabulary);

#endif
