#include <htmlcxx/html/ParserDom.h>
#include "collection_processor.h"

using namespace htmlcxx;
using namespace std;

void CollectionProcessor::process() {
  HTML::ParserDom parser;

  while (document_source_->hasNext()) {
    document_source_->next();
    string url = document_source_->getUrl();
    indexer_->beginDocument(url);

    string html = document_source_->getText();
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    for (; it != end; ++it) {
      if ((!it->isTag()) && (!it->isComment())) {
        string term = it->text();
        indexer_->addTerm(term);
      }
    }
  }

  indexer_->end();
}

