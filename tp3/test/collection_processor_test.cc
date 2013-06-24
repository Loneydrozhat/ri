#include <string>
#include "gtest/gtest.h"
#include "collection_processor.h"
#include "indexer_mock.h"
#include "document_source_mock.h"

using namespace std;
using ::testing::InSequence;
using ::testing::Return;

TEST(CollectionProcessor, shouldProcessSingleTerm) {
  DocumentSourceMock docSource;
  {
    InSequence expect;
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(true));
    EXPECT_CALL(docSource, getUrl()).WillOnce(Return("foo.bar"));
    EXPECT_CALL(docSource, getText()).WillOnce(Return("<html><body>term</body></html>"));
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(false));
  }

  IndexerMock indexer;
  {
    InSequence expect;
    EXPECT_CALL(indexer, beginDocument("foo.bar"));
    EXPECT_CALL(indexer, addTerm("term"));
    EXPECT_CALL(indexer, end());
  }

  CollectionProcessor processor(&docSource, &indexer);
  processor.process();
}

TEST(CollectionProcessor, shouldProcessMultipleTerms) {
  DocumentSourceMock docSource;
  {
    InSequence expect;
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(true));
    EXPECT_CALL(docSource, getUrl()).WillOnce(Return("foo.bar"));
    EXPECT_CALL(docSource, getText()).WillOnce(Return("<html><body>hello my friend</body></html>"));
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(false));
  }

  IndexerMock indexer;
  {
    InSequence expect;
    EXPECT_CALL(indexer, beginDocument("foo.bar"));
    EXPECT_CALL(indexer, addTerm("hello"));
    EXPECT_CALL(indexer, addTerm("my"));
    EXPECT_CALL(indexer, addTerm("friend"));
    EXPECT_CALL(indexer, end());
  }

  CollectionProcessor processor(&docSource, &indexer);
  processor.process();
}

TEST(CollectionProcessor, shouldIgnoreScriptTags) {
  DocumentSourceMock docSource;
  {
    InSequence expect;
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(true));
    EXPECT_CALL(docSource, getUrl()).WillOnce(Return("foo.bar"));
    EXPECT_CALL(docSource, getText()).WillOnce(Return("<html><body>hello<script>var x = 1;</script><SCRIPT>var x = 1;</SCRIPT></body></html>"));
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(false));
  }

  IndexerMock indexer;
  {
    InSequence expect;
    EXPECT_CALL(indexer, beginDocument("foo.bar"));
    EXPECT_CALL(indexer, addTerm("hello"));
    EXPECT_CALL(indexer, end());
  }

  CollectionProcessor processor(&docSource, &indexer);
  processor.process();
}

TEST(CollectionProcessor, shouldIgnoreCapitalization) {
  DocumentSourceMock docSource;
  {
    InSequence expect;
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(true));
    EXPECT_CALL(docSource, getUrl()).WillOnce(Return("foo.bar"));
    EXPECT_CALL(docSource, getText()).WillOnce(Return("<html><body>HeLLo</body></html>"));
    EXPECT_CALL(docSource, fetchNext()).WillOnce(Return(false));
  }

  IndexerMock indexer;
  {
    InSequence expect;
    EXPECT_CALL(indexer, beginDocument("foo.bar"));
    EXPECT_CALL(indexer, addTerm("hello"));
    EXPECT_CALL(indexer, end());
  }

  CollectionProcessor processor(&docSource, &indexer);
  processor.process();
}
