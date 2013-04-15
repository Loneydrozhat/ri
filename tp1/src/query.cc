#include <iostream>
#include "inverted_list.h"

using namespace std;

void printResults(vector<int_id> &docs) {
  cout << "Documents found: " << docs.size() << endl;
}

int main(int argc, char** argv) {
  InvertedList* reader = openInvertedList("index");

  string query;
  while (true) {
    cout << "Enter query and type enter or just type enter to quit ..." << endl << "> ";
    getline(cin, query);
    if (query == "") {
      break;
    } else {
      /*vector<string> terms;
      string term;
      stringstream ss(query);
      while (ss >> term) {
        lowerString(term);
        terms.push_back(term);
      };
      */
      vector<int_id> docs;

      reader->listDocs(docs, query);
      printResults(docs);
    }
  }

  delete reader;
  return EXIT_SUCCESS;
}
