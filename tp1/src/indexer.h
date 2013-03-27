#ifndef __INDEXER_H__
#define __INDEXER_H__

class Indexer {
  public:
    virtual void beginDocument(std::string &url) {}
    virtual void addTerm(std::string &term) {}
    virtual void end() {}
    virtual ~Indexer() {}
};

#endif
