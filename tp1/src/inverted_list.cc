#include <vector>
#include "inverted_list.h"
#include "file_handler.h"

class InvertedListImpl : public InvertedList {
  public:
    InvertedListImpl(const string &filename) {
      string baseFileName = filename;
      FileHandler* vocabularyFile = openFile(baseFileName + ".voc");
      unsigned int vocSize = vocabularyFile->readInt();
      for (unsigned int i = 0; i < vocSize; i++) {
        string term = vocabularyFile->readString();
        vocabulary_.put(term);
        int_id df = vocabularyFile->readInt();
        vocabulary_.setDf(term, df);
        pointers_.push_back(vocabularyFile->readFilePointer());
      }
      delete vocabularyFile;

      listsFile_ = openFile(baseFileName + ".idx");
    }
    virtual ~InvertedListImpl() {
      delete listsFile_;
    }
    virtual unsigned int countDocs(const string &term) {
      unordered_map<string, VocabularyEntry>::const_iterator it = vocabulary_.find(term);
      if (it == vocabulary_.end()) {
        return 0;
      }
      return it->second.df_;
    }
    virtual void findDocs(const string &term, vector<TermOccurrence> &docsList) {
      docsList.erase(docsList.begin(), docsList.end());
      unordered_map<string, VocabularyEntry>::const_iterator it = vocabulary_.find(term);
      if (it == vocabulary_.end()) {
        return;
      }
      VocabularyEntry entry = it->second;
      unsigned int docsCount = entry.df_;
      docsList.reserve(docsCount);
      listsFile_->setSeekPos(pointers_[entry.id_]);
      for (unsigned int i = 0; i < docsCount; i++) {
        TermOccurrence occurrence;
        occurrence.doc_ = listsFile_->readInt();
        occurrence.tf_ = listsFile_->readInt();
        docsList.push_back(occurrence);
      }
    }
  private:
    Vocabulary vocabulary_;
    vector<ifstream::pos_type> pointers_;
    FileHandler* listsFile_;
};

InvertedList* openInvertedList(const string &filename) {
  return new InvertedListImpl(filename);
}

class InvertedListWriterImpl : public InvertedListWriter {
  public:
    InvertedListWriterImpl(Vocabulary* vocabulary, const string &filename) {
      vocabulary_ = vocabulary;
      baseFileName_ = filename;
      listsFile_ = createFile(baseFileName_ + ".idx");
      pointers_.reserve(vocabulary->size());
    }
    virtual ~InvertedListWriterImpl() {
      delete listsFile_;
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
      FileHandler* vocabularyFile = createFile(baseFileName_ + ".voc");
      vocabularyFile->writeInt(vocabulary_->size());
      for (auto it = vocabulary_->begin(); it != vocabulary_->end(); it++) {
        VocabularyEntry &entry = it->second;
        vocabularyFile->writeString(it->first);
        vocabularyFile->writeInt(entry.df_);
        vocabularyFile->writeFilePointer(pointers_[entry.id_]);
      }
      delete vocabularyFile;
    };
  private:
    Vocabulary* vocabulary_;
    FileHandler* listsFile_;
    string baseFileName_;
    vector<ifstream::pos_type> pointers_;
    int_id currentTerm_ = 0;
};

InvertedListWriter* createInvertedListWriter(Vocabulary &vocabulary, const string &filename) {
  return new InvertedListWriterImpl(&vocabulary, filename);
}
