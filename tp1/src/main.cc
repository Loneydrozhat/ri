/**
 * Main file of project.
 * @author: Danilo Ferreira e Silva
 */
#include <iostream>
#include "document_source.h"

using namespace std;

int main(int argc, char** argv) {

  string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  string indexFileName("pagesRICompressed0.index.txt");
  DocumentSource* source = collectionArchive(inputDirectory, indexFileName);

  int i = 0;
  while(source->fetchNext()) {
    cerr << "[" << source->getUrl() << "]" << endl;
    i++;
  }
  cerr << "Total [" << i << "]" << endl;

  delete source;
  return EXIT_SUCCESS;
}
