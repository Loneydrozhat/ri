#include <sstream>
#include <iostream>
#include "document_source.h"
#include "CollectionReader.h"
#include "utils.h"

using namespace std;
using namespace RICPNS;

class CollectionArchive : public DocumentSource {
  public:
    CollectionArchive(const string &inputDirectory, const string &indexFileName) {
      reader_ = new CollectionReader(inputDirectory, indexFileName);
    }
    virtual bool fetchNext() {
      if (reader_->getNextDocument(doc_)) {
        const string text = doc_.getText();
        charset_ = identifyCharset(text);
        doc_.setText(stripHttpHeaders(text));
        return true;
      }
      return false;
    }
    virtual string getUrl() {
      return doc_.getURL();
    };
    virtual string getText() {
      return doc_.getText();
    };
    virtual int getCharset() {
      return charset_;
    };
    virtual ~CollectionArchive() {
      doc_.clear();
      delete reader_;
    }
  private:
    Document doc_;
    CollectionReader* reader_;
    int charset_;
};

DocumentSource* collectionArchive(const string &inputDirectory, const string &indexFileName) {
  return new CollectionArchive(inputDirectory, indexFileName);
}
