#include <string>
#include "gtest/gtest.h"
#include "utils.h"
#include "vocabulary.h"
#include "document_source_mock.h"
#include "indexer.h"
#include "collection_processor.h"
#include "inverted_list.h"

using namespace std;
using ::testing::InSequence;
using ::testing::Return;

class InvertedListTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    {
      InSequence expect;
      EXPECT_CALL(docSource_, fetchNext()).WillOnce(Return(true));
      EXPECT_CALL(docSource_, getUrl()).WillOnce(Return("1"));
      EXPECT_CALL(docSource_, getText()).WillOnce(Return("Pease porridge hot, pease porridge cold,"));
      EXPECT_CALL(docSource_, fetchNext()).WillOnce(Return(true));
      EXPECT_CALL(docSource_, getUrl()).WillOnce(Return("2"));
      EXPECT_CALL(docSource_, getText()).WillOnce(Return("Pease porridge in the pot, nine hot days old;"));
      EXPECT_CALL(docSource_, fetchNext()).WillOnce(Return(true));
      EXPECT_CALL(docSource_, getUrl()).WillOnce(Return("3"));
      EXPECT_CALL(docSource_, getText()).WillOnce(Return("Some like it hot, some like it cold,"));
      EXPECT_CALL(docSource_, fetchNext()).WillOnce(Return(true));
      EXPECT_CALL(docSource_, getUrl()).WillOnce(Return("4"));
      EXPECT_CALL(docSource_, getText()).WillOnce(Return("Some like it in the pot, nine days old"));
      EXPECT_CALL(docSource_, fetchNext()).WillOnce(Return(false));
    }

    indexer_ = createIndexer(10);

    CollectionProcessor processor(&docSource_, indexer_);
    processor.process();
  }

  virtual void TearDown() {
    delete indexer_;
  }

  DocumentSourceMock docSource_;
  Indexer* indexer_;
};

TEST_F(InvertedListTest, shouldCountDocs) {
  InvertedList* reader = openInvertedList("index");
  ASSERT_EQ((unsigned int) 0, reader->countDocs("foo"));
  ASSERT_EQ((unsigned int) 2, reader->countDocs("pease"));
  ASSERT_EQ((unsigned int) 2, reader->countDocs("porridge"));
  ASSERT_EQ((unsigned int) 2, reader->countDocs("like"));
  ASSERT_EQ((unsigned int) 2, reader->countDocs("cold"));

  delete reader;
}

TEST_F(InvertedListTest, shouldListDocs) {
  InvertedList* reader = openInvertedList("index");

  vector<int_id> ocurrences;
  reader->listDocs(ocurrences, "porridge");
  ASSERT_EQ((unsigned int) 2, ocurrences.size());

  ASSERT_EQ((unsigned int) 1, ocurrences[0]);
  ASSERT_EQ((unsigned int) 2, ocurrences[1]);
  
  delete reader;
}

/*
TEST_F(InvertedListTest, shouldListDocsIntersection) {

  InvertedList* reader = openInvertedList("index");

  vector<TermOccurrence> ocurrences;
  reader->listDocs(ocurrences, "hot");
  ASSERT_EQ((unsigned int) 3, ocurrences.size());

  reader->listDocsIntersection(ocurrences, "porridge");
  ASSERT_EQ((unsigned int) 2, ocurrences.size());

  ASSERT_EQ((unsigned int) 1, ocurrences[0].doc_);
  ASSERT_EQ((unsigned int) 2, ocurrences[1].doc_);

  delete reader;
}

TEST_F(InvertedListTest, shouldListDocsUnion) {

  InvertedList* reader = openInvertedList("index");

  vector<TermOccurrence> ocurrences;
  reader->listDocs(ocurrences, "like");
  ASSERT_EQ((unsigned int) 2, ocurrences.size());

  reader->listDocsUnion(ocurrences, "nine");
  ASSERT_EQ((unsigned int) 3, ocurrences.size());

  ASSERT_EQ((unsigned int) 2, ocurrences[0].doc_);
  ASSERT_EQ((unsigned int) 3, ocurrences[1].doc_);
  ASSERT_EQ((unsigned int) 4, ocurrences[2].doc_);

  delete reader;
}
*/
