#include <unistd.h>
#include <iostream>
#include <fstream>
#include "doc_db.h"
#include "links_processor.h"
#include "vocabulary.h"
#include "file_handler.h"
#include "page_ranker.h"
#include "anchor_indexer.h"
#include "heap.h"

using namespace std;

void initArgs(int argc, char** argv, string &inputDirectory, string &indexFileName, string &outputPrefix, size_t &bufferSize) {
  int opt;
  while ((opt = getopt(argc, argv, "d:i:b:o:")) != -1) {
    switch (opt) {
    case 'd':
      inputDirectory = optarg;
      break;
    case 'i':
      indexFileName = optarg;
      break;
    case 'o':
      outputPrefix = optarg;
      break;
    case 'b':
      bufferSize = atoi(optarg);
      break;
    default:
      cerr << "Usage: " << argv[0] << " [-d inputDirectory] [-i indexFileName] [-b bufferSize]" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void readDocsFile(DocDb& docDb, const string& outputPrefix) {
  string line;
  ifstream docsFile(outputPrefix + ".docs.txt");
  while (docsFile.good()) {
    getline(docsFile, line);
    if (!line.empty()) {
      size_t last = line.find_first_of(' ');
      string url = line.substr(0, last);
      docDb.add(url);
    }
  }
  docsFile.close();
}

void readVocabularyFile(Vocabulary* vocabulary, const string& outputPrefix, vector<unordered_map<int_id, int_id>>& atf) {
  FileHandler* vocabularyFile = openFile(outputPrefix + ".vocabulary.dat");
  unsigned int vocSize = vocabularyFile->readInt();
  for (unsigned int i = 0; i < vocSize; i++) {
    string term = vocabularyFile->readString();
    vocabulary->put(term);
    int_id df = vocabularyFile->readInt();
    vocabulary->setDf(term, df);
    vocabularyFile->readFilePointer();
    atf.push_back(unordered_map<int_id, int_id>());
  }
  delete vocabularyFile;
}

void writeAindex(Vocabulary& vocabulary, vector<unordered_map<int_id, int_id>>& atf, const string& outputPrefix) {
  AnchorIndexer aindexer(&vocabulary, outputPrefix);

  if (vocabulary.size() != atf.size()) {
    cout << "fudeu voc size != atf size" << endl;
  }

  for (auto termIt = vocabulary.begin(); termIt != vocabulary.end(); termIt++) {
    VocabularyEntry &entry = termIt->second;
    int_id termId = entry.id_;
    entry.df_ = atf[termId - 1].size();

    if (entry.df_ > 0) {

      aindexer.openTerm(termId);
      //cout << termIt->first << ":";

      vector<Triple> buffer;
      for (auto it = atf[termId - 1].begin(); it != atf[termId - 1].end(); it++) {
        Triple triple;
        triple.term_ = termId;
        triple.doc_ = it->first;
        triple.tf_ = it->second;
        buffer.push_back(triple);
      }
      buildMinHeap(buffer);
      while (buffer.size() > 0) {
        Triple triple = popMin(buffer);
        aindexer.writeEntry(triple.term_, triple.doc_, triple.tf_);
        //cout << " (" << triple.doc_ << ", " << triple.tf_ << ")";
      }
      //cout << endl;
    }
  }

  aindexer.close();
}

void computePageRank(DocDb& docDb, const string& outputPrefix, double q) {
  PageRanker ranker(&docDb, q);
  
  int iterations = 0;
  //double delta = 0.00000001;
  double error = ranker.computeIteration();
  for (int i = 0; i < 50; i++) {
    error = ranker.computeIteration();
    iterations++;
    cout << iterations << " " << error << endl;
  }

  ranker.print(outputPrefix);
}

int main(int argc, char** argv) {

  string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  string indexFileName("pagesRICompressed0.index.txt");
  string outputPrefix("out");
  size_t bufferSize = 1048576;
  initArgs(argc, argv, inputDirectory, indexFileName, outputPrefix, bufferSize);

  DocDb docDb;
  readDocsFile(docDb, outputPrefix);

  //Indexer* indexer = createIndexer(bufferSize, outputPrefix);

  Vocabulary vocabulary;
  vector<unordered_map<int_id, int_id>> atf;
  readVocabularyFile(&vocabulary, outputPrefix, atf);

  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);
  LinksProcessor processor(source, &vocabulary, &atf);
  cout << "Processing documents..." << endl;
  unsigned int t0 = time(NULL);
  processor.process(docDb);
  
  //computePageRank(docDb, outputPrefix, 0.5);
  //computePageRank(docDb, outputPrefix, 0.25);
  //computePageRank(docDb, outputPrefix, 0.15);

  writeAindex(vocabulary, atf, outputPrefix);

  delete source;

  unsigned int t1 = time(NULL) - t0;
  cout << "Total time: " << t1 << "s" << endl;
  //delete indexer;
  return EXIT_SUCCESS;
}
