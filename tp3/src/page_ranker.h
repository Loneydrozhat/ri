#ifndef __PAGE_RANK_H__
#define __PAGE_RANK_H__

#include "doc_db.h"
#include "utils.h"

using namespace std;

class PageRanker {
  public:
    PageRanker(DocDb* docDb) {
      docDb_ = docDb;
    }
    virtual ~PageRanker() {
      
    }
    virtual void compute() {
      
    }
  private:
    DocDb* docDb_;
};

#endif
