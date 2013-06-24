#include <iostream>
#include <sstream>
#include <htmlcxx/html/ParserDom.h>
#include "collection_processor.h"
#include "utils.h"
#include "string_tokenizer.h"

using namespace htmlcxx;
using namespace std;

void CollectionProcessor::process() {
  HTML::ParserDom parser;

  while (document_source_->fetchNext()) {
    string url = document_source_->getUrl();
    indexer_->beginDocument(url);

    string html = document_source_->getText();
    int charset = document_source_->getCharset();
    
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    for (; it != end; ++it) {
      
      if ((!it->isTag()) && (!it->isComment())) {
        string parentTag = dom.parent(it)->tagName();
        lowerString(parentTag);
        if (parentTag == "script" || parentTag == "style") {
          continue;
        }
        
        string input = it->text();
        StringTokenizer* st = stringTokenizer(&input, charset);
        string term;
        while (st->fetch(term)) {
          indexer_->addTerm(term);
          //cout << charset << " " << term << endl;
        }
        delete st;
      }
    }
  }

  indexer_->end();
}

