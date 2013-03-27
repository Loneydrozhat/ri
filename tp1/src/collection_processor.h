#ifndef __COLLECTION_PROCESSOR_H__
#define __COLLECTION_PROCESSOR_H__

#include "Document.h"
#include "indexer.h"

class DocumentSource {
  public:
    virtual bool getNextDocument(RICPNS::Document &doc) {
      return false;
    }
    virtual ~DocumentSource() {}
};

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
