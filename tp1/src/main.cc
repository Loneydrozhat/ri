/**
 * Main file of project.
 * @author: Danilo Ferreira e Silva
 */
#include "collection_processor.h"

using namespace std;

int main(int argc, char** argv) {

  string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  string indexFileName("pagesRICompressed0.index.txt");
  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);

  Indexer indexer;
  
  CollectionProcessor processor(source, &indexer);
  processor.process();
  
  delete source;
  return EXIT_SUCCESS;
}
