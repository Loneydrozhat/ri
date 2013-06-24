#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "indexer.h"
#include "file_handler.h"
#include "heap.h"
#include "inverted_list.h"
#include "vocabulary.h"

struct BucketTriple : public Triple {
  int_id bucket_;
};

struct Range {
  size_t start_;
  size_t end_;
  Range() {
    start_ = 0;
    end_ = 0;
  }
  Range(const size_t start, const size_t end) {
    start_ = start;
    end_ = end;
  }
};

class IndexerImpl : public Indexer {
  public:
    IndexerImpl(size_t bufferSize, const string &filename) {
      filename_ = filename;
      tempf_ = createFile(filename + ".buffer.dat");
      docsFile_ = createFile(filename + ".docs.dat");
      bufferSize_ = bufferSize;
      t0_ = time(NULL);
    }
    ~IndexerImpl() {
      delete tempf_;
      delete docsFile_;
    }
    virtual void beginDocument(const string &url) {
      endDocument();
      documents_.push_back(url);
    }
    virtual void addTerm(const string &term) {
      int_id id = vocabulary_.put(term);
      if (freqMap_.count(id)) {
        freqMap_[id] = freqMap_[id] + 1;
      } else {
        vocabulary_.increment(term);
        freqMap_[id] = 1;
      }
    }
    virtual void end() {
      endDocument();
      tempf_->flush();

      docsFile_->close();

      unsigned int tt = time(NULL) - t0_;
      cout << ERASE << documents_.size() <<  " processed in " << tt << "s" << endl;
      cout << "Vocabulary size: " << vocabulary_.size() << endl;
      cout << "Total terms: " << totalTriples_ << endl;

      vector<Range> buckets;
      sortBuckets(buckets);
      
      writeInvertedList(buckets);
      
    }
  private:
    vector<string> documents_;
    Vocabulary vocabulary_;
    unordered_map<int_id, int_id> freqMap_;
    FileHandler* tempf_;
    FileHandler* docsFile_;
    string filename_;
    size_t totalTriples_ = 0;
    size_t bufferSize_;
    unsigned int t0_;

    void endDocument() {
      int_id currentDoc = documents_.size();
      if (currentDoc == 0) {
        return;
      }

      unsigned int docLength = 0;
      float docNorm = 0.0;

      for (auto& entry: freqMap_) {
        int_id term = entry.first;
        int_id tf = entry.second;
        docLength += tf;
        float wt = 1 + log(tf);
        docNorm += wt * wt;

        // write triples <term, doc, tf> on temp file
        tempf_->writeInt(term);
        tempf_->writeInt(currentDoc);
        tempf_->writeInt(tf);
        totalTriples_++;
      }

      docNorm = sqrt(docNorm);

      docsFile_->writeString(documents_[currentDoc - 1]);

      //docsFile_ << documents_[currentDoc - 1] << " " << docLength << " " << docNorm << endl;

      freqMap_.erase(freqMap_.begin(), freqMap_.end());

      if (currentDoc % 16 == 0) {
        cout << ERASE << currentDoc <<  " processed" << flush;
      }
    }

    // Sort a block of triples and write in the temp file from [start, start + offset]
    void sortTriples(vector<Triple> &buffer, const size_t start, const unsigned int offset) {
      Triple triple;
      ifstream::pos_type startByte = start * 3 * sizeof(int_id);
      tempf_->setSeekPos(startByte);

      for (unsigned int i = 0; i < offset; i++) {
        triple.term_ = tempf_->readInt();
        triple.doc_ = tempf_->readInt();
        triple.tf_ = tempf_->readInt();
        buffer.push_back(triple);
      }

      buildMinHeap(buffer);
      tempf_->setSeekPos(startByte);
      for (unsigned int i = 0; i < offset; i++) {
        triple = popMin(buffer);
        tempf_->writeInt(triple.term_);
        tempf_->writeInt(triple.doc_);
        tempf_->writeInt(triple.tf_);
      }

    }

    void sortBuckets(vector<Range> &buckets) {
      unsigned int t0 = time(NULL);
      cout << "Sorting blocks..." << endl;

      vector<Triple> buffer;
      buffer.reserve(bufferSize_);
      size_t totalBuckets = totalTriples_ / bufferSize_;
      totalBuckets += totalTriples_ % bufferSize_ > 0 ? 1 : 0;

      for (size_t start = 0; start < totalTriples_; start += bufferSize_) {
        size_t offset = min(bufferSize_, totalTriples_ - start);
        sortTriples(buffer, start, offset);
        Range range(start, start + offset);
        buckets.push_back(range);
        cout << ERASE << buckets.size() <<  " of " << totalBuckets << flush;
      }
      tempf_->flush();
      unsigned int tt = time(NULL) - t0;
      cout << " in " << tt << "s" << endl;
    }

    void writeInvertedList(vector<Range> &buckets) {
      unsigned int t0 = time(NULL);
      cout << "Merging blocks..." << endl;
      
      vector<BucketTriple> buffer;
      BucketTriple triple;
      buffer.reserve(buckets.size());

      // fill the buffer with one triple from each bucket
      for (size_t i = 0; i < buckets.size(); i++) {
        readTriple(buckets[i], triple);
        triple.bucket_ = i;
        buffer.push_back(triple);
      }
      // construct a heap in the buffer
      buildMinHeap(buffer);

      InvertedListWriter* writer = createInvertedListWriter(vocabulary_, filename_);
      // pick the smallest triple, one by one, refilling the buffer
      for (size_t i = 0; buffer.size() > 0; i++) {
        BucketTriple min = buffer[0];
        //cout << min.term_ << " " << min.doc_ << " " << min.tf_ << endl;
        writer->writeEntry(min.term_, min.doc_, min.tf_);

        int_id bucket = min.bucket_;
        Range &bucketRange = buckets[bucket];
        if (bucketRange.start_ < bucketRange.end_) {
          // bucket has triples
          readTriple(bucketRange, triple);
          triple.bucket_ = bucket;
          popMinAndPush(buffer, triple);
        } else {
          // bucket is empty
          popMin(buffer);
        }

        if (i % 1000 == 0) {
          cout << ERASE << (100 * i / totalTriples_) << "%" << flush;
        }
      }
      writer->close();
      delete writer;

      unsigned int tt = time(NULL) - t0;
      cout << ERASE << "Done in " << tt << "s" << endl;
    }

    void readTriple(Range &range, BucketTriple &triple) {
      ifstream::pos_type startByte = range.start_ * 3 * sizeof(int_id);
      tempf_->setSeekPos(startByte);
      triple.term_ = tempf_->readInt();
      triple.doc_ = tempf_->readInt();
      triple.tf_ = tempf_->readInt();
      range.start_ = range.start_ + 1;
    }
};

Indexer* createIndexer(size_t bufferSize, const string &filename) {
  return new IndexerImpl(bufferSize, filename);
}
