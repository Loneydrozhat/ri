#include <unistd.h>
#include <iostream>
#include <fstream>
#include "doc_db.h"
#include "links_processor.h"
#include "vocabulary.h"
#include "file_handler.h"
#include "page_ranker.h"

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

  // TODO remover
  /*
  size_t n = docDb->size();

  unsigned int maxInDegree = 0;
  unsigned int inDegreeSum = 0;
  unsigned int outDegreeSum = 0;
  unsigned int inDegreeCount = 0;
  for (unsigned int i = 0; i < n; i++) {
    DocDbEntry& docEntry = docDb->get(i + 1);
    vector<int_id>& inlinks = docEntry.inlinks;
    size_t inlinkCount = inlinks.size();
    maxInDegree = (inlinkCount > maxInDegree) ? inlinkCount : maxInDegree;
    inDegreeSum += docEntry.inlinksCount;
    outDegreeSum += docEntry.validoutlinks;
    if (inlinkCount > 0) {
      inDegreeCount++;
    }
  }

  cout << n << " documents" << endl;
  cout << "Max in-degree is " << maxInDegree << endl;
  cout << "Mean in-degree is " << inDegreeSum << endl;
  cout << "Mean out-degree is " << outDegreeSum << endl;
  cout << inDegreeCount << " documents have in-degree > 0" << endl;
  */
}

void readVocabularyFile(Vocabulary* vocabulary, const string& outputPrefix) {
  FileHandler* vocabularyFile = openFile(outputPrefix + ".vocabulary.dat");
  unsigned int vocSize = vocabularyFile->readInt();
  for (unsigned int i = 0; i < vocSize; i++) {
    string term = vocabularyFile->readString();
    vocabulary->put(term);
    int_id df = vocabularyFile->readInt();
    vocabulary->setDf(term, df);
    vocabularyFile->readFilePointer();
  }
  delete vocabularyFile;
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
  readVocabularyFile(&vocabulary, outputPrefix);

  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);
  LinksProcessor processor(source, &vocabulary);
  cout << "Processing documents..." << endl;
  unsigned int t0 = time(NULL);
  processor.process(docDb);

  //docDb.linkDocuments(52, 53);
  //docDb.linkDocuments(53, 52);

  PageRanker ranker(&docDb, 0.15);
  
  int iterations = 0;
  //double delta = 0.00000001;
  double error = ranker.computeIteration();
  for (int i = 0; i < 50; i++) {
    error = ranker.computeIteration();
    iterations++;
    cout << iterations << " " << error << endl;
  }

  ranker.print(outputPrefix);

  delete source;

  unsigned int t1 = time(NULL) - t0;
  cout << "Total time: " << t1 << "s" << endl;
  //delete indexer;
  return EXIT_SUCCESS;
}
