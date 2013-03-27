#include <string>
#include "gtest/gtest.h"
#include "collection_processor.h"

using namespace std;
using namespace RICPNS;

class FakeDocumentSource : public DocumentSource {
  public:
    FakeDocumentSource() {
      has_next_ = true;
    }
    virtual bool getNextDocument(Document &doc) {
      if (has_next_) {
        string url = "foo.bar";
        string text = "<html><body>Term</body></html>";
        doc.setURL(url);
        doc.setText(text);
        has_next_ = false;
      }
      return false;
    }
  private:
    bool has_next_ = true;
};

class CollectionProcessorTest : public testing::Test {
  protected:
    virtual void SetUp() {
      indexer_ = new Indexer();
      doc_source_ = new FakeDocumentSource();
      processor_ = new CollectionProcessor(doc_source_, indexer_);
    }
    virtual void TearDown() {
      delete indexer_;
      delete doc_source_;
      delete processor_;
    }
    Indexer* indexer_;
    DocumentSource* doc_source_;
    CollectionProcessor* processor_;
};

TEST_F(CollectionProcessorTest, shouldRun) {
  processor_->process();
  //EXPECT_EQ("Hello, world!", "Hello, world!");
}
