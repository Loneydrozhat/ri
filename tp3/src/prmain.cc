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


  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);
  
  DocDb docDb;
  while (source->fetchNext()) {
    string url = source->getUrl();
    docDb.add(url);
  }
  delete source;

  DocumentSource* source2 = collectionArchive(inputDirectory, indexFileName);
  //Indexer* indexer = createIndexer(bufferSize, outputPrefix);

  Vocabulary vocabulary;
  readVocabularyFile(&vocabulary, outputPrefix);

  LinksProcessor processor(source2, &docDb, &vocabulary);
  cout << "Processing documents..." << endl;
  unsigned int t0 = time(NULL);
  processor.process();
  unsigned int t1 = time(NULL) - t0;
  cout << "Total time: " << t1 << "s" << endl;

  PageRanker ranker(&docDb);
  ranker.compute();

  delete source2;
  //delete indexer;
  return EXIT_SUCCESS;
}
