#ifndef __DOCUMENT_SOURCE_H__
#define __DOCUMENT_SOURCE_H__

#include <string>

using namespace std;

class DocumentSource {
  public:
    virtual bool fetchNext() = 0;
    virtual string getUrl() = 0;
    virtual string getText() = 0;
    virtual int getCharset() = 0;
    virtual ~DocumentSource() {}
};

DocumentSource* collectionArchive(const string &inputDirectory, const string &indexFileName);

string stripHttpHeaders(const string &text);

#endif
