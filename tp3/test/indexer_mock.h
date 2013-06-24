#ifndef __INDEXER_MOCK_H__
#define __INDEXER_MOCK_H__

#include "gmock/gmock.h"

class IndexerMock : public Indexer {
  public:
    MOCK_METHOD1(beginDocument, void(const std::string &url));
    MOCK_METHOD1(addTerm, void(const std::string &term));
    MOCK_METHOD0(end, void());
};

#endif
