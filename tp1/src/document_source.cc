#include <sstream>
#include <iostream>
#include "document_source.h"
#include "CollectionReader.h"

using namespace std;
using namespace RICPNS;

class CollectionArchive : public DocumentSource {
  public:
    CollectionArchive(const string &inputDirectory, const string &indexFileName) {
      reader = new CollectionReader(inputDirectory, indexFileName);
    }
    virtual bool fetchNext() {
      if (reader->getNextDocument(doc)) {
        doc.setText(stripHttpHeaders(doc.getText()));
        return true;
      }
      return false;
    }
    virtual string getUrl() {
      return doc.getURL();
    };
    virtual string getText() {
      return doc.getText();
    };
    virtual ~CollectionArchive() {
      doc.clear();
      delete reader;
    }
  private:
    Document doc;
    CollectionReader* reader;
};

DocumentSource* collectionArchive(const string &inputDirectory, const string &indexFileName) {
  return new CollectionArchive(inputDirectory, indexFileName);
}
