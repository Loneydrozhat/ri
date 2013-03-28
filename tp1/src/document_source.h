#ifndef __DOCUMENT_SOURCE_H__
#define __DOCUMENT_SOURCE_H__

using namespace std;

class DocumentSource {
  public:
    virtual bool hasNext() {
      return false;
    }
    virtual void next() {}
    virtual string getUrl() {
      return NULL;
    }
    virtual string getText() {
      return NULL;
    }
    virtual ~DocumentSource() {}
};

#endif
