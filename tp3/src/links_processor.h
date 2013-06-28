#ifndef __LINKS_PROCESSOR_H__
#define __LINKS_PROCESSOR_H__

#include "document_source.h"
#include "indexer.h"
#include "doc_db.h"
#include "vocabulary.h"

using namespace std;

class LinksProcessor {
  public:
    LinksProcessor(DocumentSource* documentSource, Vocabulary* vocabulary, vector<unordered_map<int_id, int_id>>* atf) {
      document_source_ = documentSource;
      vocabulary_ = vocabulary;
      atf_ = atf;
    }
    virtual void process(DocDb& docDb);
    virtual void emitTerms(const string& text, int charset, int_id docId);
    virtual ~LinksProcessor() {}
  protected:
    DocumentSource* document_source_;
    Vocabulary* vocabulary_;
    vector<unordered_map<int_id, int_id>>* atf_;
};

#endif
