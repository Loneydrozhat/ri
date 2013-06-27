#ifndef __PAGE_RANK_H__
#define __PAGE_RANK_H__

#include <vector>
#include <cmath>
#include <fstream>
#include "doc_db.h"
#include "utils.h"

using namespace std;

class PageRanker {
  public:
    PageRanker(DocDb* docDb, double q) {
      q_ = q;
      docDb_ = docDb;
      size_t n = docDb_->size();
      iteration_ = 0;
      double value = 1.0;// / n;
      pr1_ = new vector<double>(n, value);
      pr2_ = new vector<double>(n, value);

      
      unsigned int maxInDegree = 0;
      unsigned int maxOutDegree = 0;
      unsigned int inDegreeSum = 0;
      unsigned int outDegreeSum = 0;
      unsigned int inDegreeCount = 0;
      for (unsigned int i = 0; i < n; i++) {
        DocDbEntry& docEntry = docDb_->get(i + 1);
        size_t inlinkCount = docEntry.inDegree();
        size_t outlinkCount = docEntry.outDegree();
        maxInDegree = (inlinkCount > maxInDegree) ? inlinkCount : maxInDegree;
        maxOutDegree = (outlinkCount > maxOutDegree) ? outlinkCount : maxOutDegree;
        inDegreeSum = inDegreeSum + inlinkCount;
        //cout << (i+1) << ": in=" << inlinkCount << ", out=" << docEntry.outDegree() << endl;
        outDegreeSum = outDegreeSum + docEntry.outDegree();
        if (inlinkCount > 0) {
          inDegreeCount++;
        }
        // TODO
        /*
        if (inlinkCount == 49) {
          cout << "R49" << endl;
          for (unsigned int j = 0; j < inlinkCount; j++) {
            cout << inlinks[j] << endl;
          }
        }
        */
      }

      cout << n << " documents" << endl;
      cout << docDb_->numLinks << " intra collection links" << endl;
      cout << "Max in-degree is " << maxInDegree << endl;
      cout << "Max out-degree is " << maxOutDegree << endl;
      //cout << "Mean in-degree is " << inDegreeSum << endl;
      //cout << "Mean out-degree is " << outDegreeSum << endl;
      cout << inDegreeCount << " documents have in-degree > 0" << endl;
      

    }
    virtual ~PageRanker() {
      delete pr1_;
      delete pr2_;
    }
    virtual double computeIteration() {
      vector<double>* swap = pr1_;
      pr1_ = pr2_;
      pr2_ = swap;

      double meanSqrtErr = 0.0;
      size_t n = docDb_->size();
      double factor = 1.0 / n;
      for (unsigned int i = 0; i < n; i++) {
        double pr = computePr(i, n);
        pr2_->at(i) = pr;
        double err = pr2_->at(i) - pr1_->at(i);
        meanSqrtErr += factor * err * err;
      }

      //cout << "errsum " << meanSqrtErr << endl;
      return meanSqrtErr;
    }
    virtual void print(const string& filePrefix) {
      ofstream prfile;
      prfile.open(filePrefix + ".pr.txt");
      size_t n = docDb_->size();
      for (unsigned int i = 0; i < n; i++) {
        DocDbEntry& docEntry = docDb_->get(i + 1);
        double pr = pr2_->at(i);
        prfile << docEntry.inDegree() << " " << pr << endl;
      }
      prfile.close();
    }
    virtual double getPr(unsigned int i) {
      return pr2_->at(i - 1);
    }
  private:
    int iteration_;
    vector<double>* pr1_;
    vector<double>* pr2_;
    double q_ = 0.15;
    DocDb* docDb_;

    double computePr(unsigned int i, unsigned int n) {
      int_id docId = i + 1;
      DocDbEntry &docEntry = docDb_->get(docId);

      //bool passFlow = docEntry.validoutlinks > 0 && inlinks.size() > 0;
      //if (passFlow) {
      
      double pr = q_;
      //cout << "pr(" << i << ") = " << q_ << " + " << (1 - q_) << " * (";
      double sum = 0.0;

      unordered_set<int_id>::iterator end = docEntry.inLinksEnd();
      for (unordered_set<int_id>::iterator it = docEntry.inLinksBegin(); it != end; it++) {
        int_id jDocId = (*it);
        DocDbEntry &jDocEntry = docDb_->get(jDocId);
        sum += pr1_->at(jDocId - 1) / jDocEntry.outDegree();
        //cout << " + " << pr1_->at(jDocId - 1) << " / " << jDocEntry.validoutlinks;
      }
      pr += (1 - q_) * sum;
      //cout << ")" << endl;

      return pr;
      //}

      //return pr1_->at(i);

      //if (pr == 1.0) {
      //  cout << inlinks.size();
      //  for (int j = 0, len = inlinks.size(); j < len; j++) {
      //    int_id jDocId = inlinks.at(j);
      //    DocDbEntry &jDocEntry = docDb_->get(jDocId);
      //    cout << " ref " << pr1_->at(jDocId - 1) << " " << jDocEntry.backrefs.size() << " " << jDocEntry.outlinks;
      //  }
      //  cout << endl;
      //}

    }
};

#endif
