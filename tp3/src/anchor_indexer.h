#ifndef __ANCHOR_INDEXER_H__
#define __ANCHOR_INDEXER_H__

#include <string>
#include <vector>
#include "utils.h"
#include "vocabulary.h"
#include "file_handler.h"

using namespace std;

class AnchorIndexer {
  public:
    AnchorIndexer(Vocabulary* vocabulary, const string &filename) {
      vocabulary_ = vocabulary;
      baseFileName_ = filename;
      listsFile_ = createFile(baseFileName_ + ".aindex.dat");
      pointers_.reserve(vocabulary->size());
    }
    virtual ~AnchorIndexer() {
      delete listsFile_;
    }
    virtual void openTerm(int_id term) {
      ifstream::pos_type seekp = listsFile_->getSeekPos();
      pointers_[term] = seekp;
      //.push_back(seekp);
    }
    virtual void writeEntry(int_id term, int_id doc, int_id tf) {
      //if (currentTerm_ != term) {
      //  currentTerm_ = term;
      //  ifstream::pos_type seekp = listsFile_->getSeekPos();
        //if (seekp == seekp_) {
        // danilo
          //cout << term << " FUDEU " << seekp_ << " " << seekp << endl;
        //}
        //pointers_.push_back(seekp);
      //}

      listsFile_->writeInt(doc);
      listsFile_->writeInt(tf);
      seekp_ += 2 * sizeof(int_id);
    }
    virtual void close() {
      listsFile_->close();
      FileHandler* vocabularyFile = createFile(baseFileName_ + ".avocabulary.dat");
      vocabularyFile->writeInt(vocabulary_->size());
      unsigned int count = 0;
      for (auto it = vocabulary_->begin(); it != vocabulary_->end(); it++) {
        VocabularyEntry &entry = it->second;
        if (entry.df_ > 0) {
          count++;
          vocabularyFile->writeString(it->first);
          //cout << it->first << endl;
          vocabularyFile->writeInt(entry.df_);
          vocabularyFile->writeFilePointer(pointers_[entry.id_]);
          // danilo
          //cout << it->first << " " << entry.df_ << " " << pointers_[entry.id_] << endl;
        }
      }
      vocabularyFile->setSeekPos(0);
      vocabularyFile->writeInt(count);

      vocabularyFile->close();
      delete vocabularyFile;
    };
  private:
    Vocabulary* vocabulary_;
    FileHandler* listsFile_;
    string baseFileName_;
    unordered_map<int_id, ifstream::pos_type> pointers_;
    ifstream::pos_type seekp_ = 0;
    //int_id currentTerm_ = 0;
};

#endif
