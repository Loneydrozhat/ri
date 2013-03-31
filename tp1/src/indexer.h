#ifndef __INDEXER_H__
#define __INDEXER_H__

#include <iostream>

using namespace std;

class Indexer {
  public:
    virtual void beginDocument(const string &url) {
      cout << url << endl;
    }
    virtual void addTerm(const string &term) {
      cout << term << endl;
    }
    virtual void end() {}
    virtual ~Indexer() {}
};

#endif
