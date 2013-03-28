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
      return reader->getNextDocument(doc);
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
    bool hasNext = false;
};

DocumentSource* collectionArchive(const string &inputDirectory, const string &indexFileName) {
  return new CollectionArchive(inputDirectory, indexFileName);
}
