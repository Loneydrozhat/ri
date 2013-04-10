/**
 * Main file of project.
 * @author: Danilo Ferreira e Silva
 */
#include <iostream>
#include "collection_processor.h"

using namespace std;

int main(int argc, char** argv) {

  //string inputDirectory("/media/danilo/Users/Danilo/Workspaces/ri2013/data");
  //string indexFileName("index-small.txt");

  string inputDirectory("/media/danilo/Users/Danilo/Workspaces/ri2013/data");
  string indexFileName("index.txt");

  //string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  //string indexFileName("pagesRICompressed0.index.txt");

  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);
  Indexer* indexer = createIndexer();
  
  CollectionProcessor processor(source, indexer);
  cout << "Processing documents..." << endl;
  unsigned int t0 = time(NULL);
  processor.process();
  unsigned int t1 = time(NULL) - t0;
  cout << "Elapsed time " << t1 << "s" << endl;
  
  delete source;
  delete indexer;
  return EXIT_SUCCESS;
}
