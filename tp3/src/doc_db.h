#ifndef __DOC_DB_H__
#define __DOC_DB_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

using namespace std;

class DocDbEntry {
  public:
    DocDbEntry() {
      outDegree_ = 0;
    }
    //vector<int_id> inlinks;
    //int outlinks = 0;
    //unsigned int validoutlinks = 0;
    size_t outDegree() {
      return outDegree_;
    }
    size_t inDegree() {
      return inlinks_.size();
    }
    unordered_set<int_id>::iterator inLinksBegin() {
      return inlinks_.begin();
    }
    unordered_set<int_id>::iterator inLinksEnd() {
      return inlinks_.end();
    }
    bool addInLink(int_id docId) {
      pair<unordered_set<int_id>::iterator, bool> result = inlinks_.insert(docId);
      return result.second;
    }
    void incrementOutDegree() {
      outDegree_++;
    }
    void addTerm(int_id termId) {
      for (int i = terms_.size() - 1; i >= 0; i--) {
        if (termId == terms_[i]) {
          return;
        }
      }
      terms_.push_back(termId);
    }
    size_t termsCount() {
      return terms_.size();
    }
  private:
    unordered_set<int_id> inlinks_;
    vector<int_id> terms_;
    size_t outDegree_;
};

class DocDb {
  public:
    virtual void add(const string &url) {
      int_id docId = docs_.size() + 1;
      docs_.push_back(DocDbEntry());
      if (urls_.count(url) == 0) {
        urls_[url] = docId;
      } else {
        cout << "WARN " << docId << " duplicado" << endl;
      }
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
    
    virtual void linkDocuments(int_id docSource, int_id docTarget) {
      if (docSource == docTarget) {
        cout << "WARN docs iguais" << endl;
      }
      //cout << docSource << " -> " << docTarget << endl;
      //DocDbEntry& source = get(docSource);
      //DocDbEntry& target = get(docTarget);
      //target.inlinks.push_back(docSource);
      //source.validoutlinks++;
      bool inserted = docs_[docTarget-1].addInLink(docSource);
      if (inserted) {
        docs_[docSource-1].incrementOutDegree();
        numLinks++;
      }

      //cout << docSource << ": in=" << docs_[docSource-1].inDegree() << ", out=" << docs_[docSource-1].outDegree() << endl;
      //cout << docTarget << ": in=" << docs_[docTarget-1].inDegree() << ", out=" << docs_[docTarget-1].outDegree() << endl;
    }
    size_t numLinks = 0;

  private:
    vector<DocDbEntry> docs_;
    unordered_map<string, int_id> urls_;
};


#endif
