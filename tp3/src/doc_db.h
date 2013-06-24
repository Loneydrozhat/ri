#ifndef __DOC_DB_H__
#define __DOC_DB_H__

#include <string>
#include <vector>
#include <unordered_map>
#include "utils.h"

using namespace std;

struct DocDbEntry {
  vector<int_id> backrefs;
  vector<int_id> terms;
  int outlinks = 0;
  void addBackref(int_id docId) {
    backrefs.push_back(docId);
  }
  void addTerm(int_id termId) {
    for (int i = terms.size() - 1; i >= 0; i--) {
      if (termId == terms[i]) {
        return;
      }
    }
    terms.push_back(termId);
  }
  unsigned int backrefsCount() {
    return backrefs.size();
  }
  vector<int_id>& getBackrefs() {
    return backrefs;
  }
  vector<int_id>& getTerms() {
    return terms;
  }
};

class DocDb {
  public:
    virtual void add(const string &url) {
      int_id docId = docs_.size() + 1;
      DocDbEntry newDoc;
      docs_.push_back(newDoc);
      urls_[url] = docId;
    }
    virtual int_id getId(const string &url) {
      if (urls_.count(url)) {
        return urls_[url];
      }
      return 0;
    }
    virtual DocDbEntry& get(int_id id) {
      return docs_.at(id - 1);
    }
    virtual size_t size() {
      return docs_.size();
    }
  private:
    vector<DocDbEntry> docs_;
    unordered_map<string, int_id> urls_;
};


#endif
