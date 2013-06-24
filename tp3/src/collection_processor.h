#ifndef __COLLECTION_PROCESSOR_H__
#define __COLLECTION_PROCESSOR_H__

#include "document_source.h"
#include "indexer.h"

using namespace std;

class CollectionProcessor {
  public:
    CollectionProcessor(DocumentSource* documentSource, Indexer* indexer) {
      document_source_ = documentSource;
      indexer_ = indexer;
    }
    virtual void process();
    virtual ~CollectionProcessor() {}
  protected:
    DocumentSource* document_source_;
    Indexer* indexer_;
};

#endif
