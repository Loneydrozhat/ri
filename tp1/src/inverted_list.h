#ifndef __INVERTED_LIST_H__
#define __INVERTED_LIST_H__

using namespace std;

class Indexer {
  public:
    virtual void beginDocument(const string &url) {};
    virtual void addTerm(const string &term) {};
    virtual void end() {};
    virtual ~Indexer() {}
};

Indexer* createIndexer();

#endif
