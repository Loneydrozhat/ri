#ifndef __DOCUMENT_SOURCE_MOCK_H__
#define __DOCUMENT_SOURCE_MOCK_H__

#include "document_source.h"
#include "gmock/gmock.h"

class DocumentSourceMock : public DocumentSource {
  public:
    MOCK_METHOD0(fetchNext, bool());
    MOCK_METHOD0(getUrl, string());
    MOCK_METHOD0(getText, string());
    MOCK_METHOD0(getCharset, int());
};

#endif
