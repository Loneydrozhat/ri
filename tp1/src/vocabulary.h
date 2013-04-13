#ifndef __VOCABULARY_H__
#define __VOCABULARY_H__

#include <string>
#include <unordered_map>
#include "utils.h"

using namespace std;

struct VocabularyEntry {
  int_id id_;
  int_id df_;
  VocabularyEntry() {
    id_ = 0;
    df_ = 0;
  }
};

class Vocabulary {
  public:
    virtual int_id put(const string &term) {
      VocabularyEntry &e = map_[term];
      if (e.id_ == 0) {
        e.id_ = map_.size();
      }
      return e.id_;
    }
    virtual unordered_map<string, VocabularyEntry>::const_iterator find(const string &term) {
      return map_.find(term);
    }
    virtual void increment(const string &term) {
      VocabularyEntry &e = map_[term];
      e.df_++;
    }
    virtual void setDf(const string &term, int_id df) {
      VocabularyEntry &e = map_[term];
      e.df_ = df;
    }
    virtual size_t size() {
      return map_.size();
    }
    virtual unordered_map<string, VocabularyEntry>::iterator begin() {
      return map_.begin();
    }
    virtual unordered_map<string, VocabularyEntry>::iterator end() {
      return map_.end();
    }
  private:
    unordered_map<string, VocabularyEntry> map_;
};


#endif
