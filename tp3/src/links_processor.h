#ifndef __LINKS_PROCESSOR_H__
#define __LINKS_PROCESSOR_H__

#include "document_source.h"
#include "indexer.h"
#include "doc_db.h"

using namespace std;

class LinksProcessor {
  public:
    LinksProcessor(DocumentSource* documentSource, Indexer* indexer, DocDb* docDb) {
      document_source_ = documentSource;
      indexer_ = indexer;
      docDb_ = docDb;
    }
    virtual void process();
    virtual ~LinksProcessor() {}
  protected:
    DocumentSource* document_source_;
    Indexer* indexer_;
    DocDb* docDb_;
};

#endif
