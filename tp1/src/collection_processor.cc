#include <htmlcxx/html/ParserDom.h>
#include "collection_processor.h"

using namespace RICPNS;
using namespace htmlcxx;
using namespace std;

void CollectionProcessor::process() {
  HTML::ParserDom parser;
  Document doc;
  doc.clear();

  while (document_source_->getNextDocument(doc)) {
    string url = doc.getURL();
    indexer_->beginDocument(url);

    string html = doc.getText();
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    for (; it != end; ++it) {
      if ((!it->isTag()) && (!it->isComment())) {
        string term = it->text();
        indexer_->addTerm(term);
      }
    }

    doc.clear();
  }

  indexer_->end();
}

