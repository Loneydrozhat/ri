/**
 * Main file of project.
 * @author: Danilo Ferreira e Silva
 */
#include "CollectionReader.h"

#include <cstdlib>
#include <iostream>

using namespace std;
using namespace RICPNS;

int main(int argc, char** argv) {

  string inputDirectory("/home/danilo/workspaces/ri/tp1/data");
  string indexFileName("pagesRICompressed0.index.txt");

  CollectionReader * reader = new CollectionReader(inputDirectory, indexFileName);
  Document doc;
  doc.clear();
  int i = 0;
  while(reader->getNextDocument(doc)) {
    cerr << "[" << doc.getURL() << "]" << endl;
    doc.clear();
    ++i;
  }
  cerr << "Total [" << i << "]" << endl;

  delete reader;

  return EXIT_SUCCESS;
}
