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
    IndexerImpl() {
      tempf_ = createFile("buffer.tmp");
    }
    ~IndexerImpl() {
      delete tempf_;
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
      //e.tf_++;
    }
    virtual void end() {
      endDocument();
      cout << ERASE << documents_.size() <<  " processed" << endl;
      cout << vocabulary_.size() << " distinct terms" << endl;
      cout << totalTriples_ << " triples written to temp file" << endl;

      vector<Range> buckets;
      sortBuckets(buckets);
      
      cout << "Merging blocks..." << endl;
      writeInvertedList(buckets);
      cout << "Inverted lists generated" << endl;
    }
  private:
    vector<string> documents_;
    Vocabulary vocabulary_;
    unordered_map<int_id, int_id> freqMap_;
    FileHandler* tempf_;
    size_t totalTriples_ = 0;
    size_t bucketSize_ = 1048576;

    void endDocument() {
      int_id currentDoc = documents_.size();
      if (currentDoc == 0) {
        return;
      }

      for (auto& entry: freqMap_) {
        // write triples <term, doc, tf> on temp file
        tempf_->writeInt(entry.first);
        tempf_->writeInt(currentDoc);
        tempf_->writeInt(entry.second);
        totalTriples_++;
      }

      freqMap_.erase(freqMap_.begin(), freqMap_.end());

      if (currentDoc % 10 == 0) {
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
      cout << "Sorting blocks..." << endl;

      vector<Triple> buffer;
      buffer.reserve(bucketSize_);
      size_t totalBuckets = totalTriples_ / bucketSize_;
      totalBuckets += totalTriples_ % bucketSize_ > 0 ? 1 : 0;

      for (size_t start = 0; start < totalTriples_; start += bucketSize_) {
        size_t offset = min(bucketSize_, totalTriples_ - start);
        sortTriples(buffer, start, offset);
        Range range(start, start + offset);
        buckets.push_back(range);
        cout << ERASE << buckets.size() <<  " of " << totalBuckets << flush;
      }
      cout << endl;
    }

    void writeInvertedList(vector<Range> &buckets) {
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

      InvertedListWriter* writer = createInvertedListWriter(vocabulary_, "index");
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

      cout << ERASE << "100%" << endl;
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

Indexer* createIndexer() {
  return new IndexerImpl();
}
