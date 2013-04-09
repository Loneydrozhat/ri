#include "inverted_list.h"

class InvertedListWriterImpl : public InvertedListWriter {
  public:
    InvertedListWriterImpl(const string &file) {}
    virtual ~InvertedListWriterImpl() {}
    virtual void writeEntry(int_id term, int_id doc, int_id tf) {

    }
    virtual void close() {

    };
};

InvertedListWriter* createInvertedListWriter() {
  return new InvertedListWriterImpl("index");
}