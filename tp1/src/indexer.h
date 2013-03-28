#ifndef __INDEXER_H__
#define __INDEXER_H__

class Indexer {
  public:
    virtual void beginDocument(const std::string &url) {}
    virtual void addTerm(const std::string &term) {}
    virtual void end() {}
    virtual ~Indexer() {}
};

#endif
