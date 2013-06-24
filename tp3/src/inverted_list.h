#ifndef __INVERTED_LIST_H__
#define __INVERTED_LIST_H__

#include <string>
#include <vector>
#include "utils.h"
#include "vocabulary.h"

using namespace std;

class InvertedList {
  public:
    virtual ~InvertedList() {}
    virtual unsigned int countDocs(const string &term) = 0;
    virtual void listDocs(vector<int_id> &docsList, const string &term) = 0;
};

InvertedList* openInvertedList(const string &filename);

class InvertedListWriter {
  public:
    virtual ~InvertedListWriter() {}
    virtual void writeEntry(int_id term, int_id doc, int_id tf) {}
    virtual void close() {}
};

InvertedListWriter* createInvertedListWriter(Vocabulary &vocabulary, const string &filename);

#endif
