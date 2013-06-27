#ifndef __LINKS_PROCESSOR_H__
#define __LINKS_PROCESSOR_H__

#include "document_source.h"
#include "indexer.h"
#include "doc_db.h"
#include "vocabulary.h"

using namespace std;

class LinksProcessor {
  public:
    LinksProcessor(DocumentSource* documentSource, Vocabulary* vocabulary) {
      document_source_ = documentSource;
      vocabulary_ = vocabulary;
    }
    virtual void process(DocDb& docDb);
    virtual void emitTerms(const string& text, int charset, int_id docId, DocDb& docDb);
    virtual ~LinksProcessor() {}
  protected:
    DocumentSource* document_source_;
    Vocabulary* vocabulary_;
};

#endif
