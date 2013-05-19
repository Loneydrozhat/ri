#include <unistd.h>
#include <iostream>
#include "collection_processor.h"

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


int main(int argc, char** argv) {

  string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  string indexFileName("pagesRICompressed0.index.txt");
  string outputPrefix("out");
  size_t bufferSize = 1048576;
  initArgs(argc, argv, inputDirectory, indexFileName, outputPrefix, bufferSize);

  //string inputDirectory("/media/danilo/Users/Danilo/Workspaces/ri2013/data");
  //string indexFileName("index-5k.txt");

  //string inputDirectory("/media/danilo/Users/Danilo/Workspaces/ri2013/data");
  //string indexFileName("index.txt");


  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);
  Indexer* indexer = createIndexer(bufferSize, outputPrefix);
  
  CollectionProcessor processor(source, indexer);
  cout << "Processing documents..." << endl;
  unsigned int t0 = time(NULL);
  processor.process();
  unsigned int t1 = time(NULL) - t0;
  cout << "Total time: " << t1 << "s" << endl;
  
  delete source;
  delete indexer;
  return EXIT_SUCCESS;
}
