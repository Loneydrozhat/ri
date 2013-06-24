#ifndef __LINKS_PROCESSOR_H__
#define __LINKS_PROCESSOR_H__

#include "document_source.h"
#include "indexer.h"
#include "doc_db.h"
#include "vocabulary.h"

using namespace std;

class LinksProcessor {
  public:
    LinksProcessor(DocumentSource* documentSource, DocDb* docDb, Vocabulary* vocabulary) {
      document_source_ = documentSource;
      docDb_ = docDb;
      vocabulary_ = vocabulary;
    }
    virtual void process();
    virtual ~LinksProcessor() {}
  protected:
    DocumentSource* document_source_;
    DocDb* docDb_;
    Vocabulary* vocabulary_;
};

#endif
