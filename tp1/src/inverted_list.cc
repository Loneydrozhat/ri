#include <vector>
#include "inverted_list.h"
#include "file_handler.h"

class InvertedListWriterImpl : public InvertedListWriter {
  public:
    InvertedListWriterImpl(Vocabulary* vocabulary) {
      vocabulary_ = vocabulary;
      listsFile_ = createFile("ilists");
      vocabularyFile_ = createFile("ivoc");
      pointers_.reserve(vocabulary->size());
    }
    virtual ~InvertedListWriterImpl() {
      delete listsFile_;
      delete vocabularyFile_;
    }
    virtual void writeEntry(int_id term, int_id doc, int_id tf) {
      if (currentTerm_ != term) {
        currentTerm_ = term;
        ifstream::pos_type seekp = listsFile_->getSeekPos();
        pointers_.push_back(seekp);
      }
      listsFile_->writeInt(doc);
      listsFile_->writeInt(tf);
    }
    virtual void close() {
      for (auto it = vocabulary_->begin(); it != vocabulary_->end(); it++) {
        VocabularyEntry &entry = it->second;
        vocabularyFile_->writeInt(pointers_[entry.id_]);
        vocabularyFile_->writeInt(entry.df_);
        vocabularyFile_->writeString(it->first);
      }
    };
  private:
    Vocabulary* vocabulary_;
    FileHandler* listsFile_;
    FileHandler* vocabularyFile_;
    vector<ifstream::pos_type> pointers_;
    int_id currentTerm_ = 0;
};

InvertedListWriter* createInvertedListWriter(Vocabulary &vocabulary) {
  return new InvertedListWriterImpl(&vocabulary);
}
