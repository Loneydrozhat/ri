#ifndef __INDEXER_H__
#define __INDEXER_H__

#include "utils.h"

using namespace std;

struct Triple {
  int_id term_;
  int_id doc_;
  int_id tf_;
  int compareTo(Triple &t2) {
    int diff = term_ - t2.term_;
    if (diff == 0) {
      diff = doc_ - t2.doc_;
    }
    return diff;
  }
  friend bool operator> (Triple &t1, Triple &t2) {
    return t1.compareTo(t2) > 0;
  }
  friend bool operator< (Triple &t1, Triple &t2) {
    return t1.compareTo(t2) < 0;
  }
};

class Indexer {
  public:
    virtual void beginDocument(const string &url) {};
    virtual void addTerm(const string &term) {};
    virtual void end() {};
    virtual ~Indexer() {}
};

Indexer* createIndexer(size_t bufferSize);

#endif
