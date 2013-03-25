#ifndef __COLLECTION_PROCESSER_H__
#define __COLLECTION_PROCESSER_H__

class DocumentSource;
class Indexer;
class EventLogger;

class CollectionProcesser {
  public:
    CollectionProcesser(const DocumentSource &documentSource, const Indexer &indexer, const EventLogger &eventLogger) {
      document_source_ = &documentSource;
      indexer_ = &indexer;
      event_logger_ = &eventLogger;
    }
    void process();
  private:
    DocumentSource* document_source_;
    Indexer* indexer_;
    EventLogger* event_logger_;
};

#endif
