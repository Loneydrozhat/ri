#include <iostream>
#include <sstream>
#include <htmlcxx/html/ParserDom.h>
#include "links_processor.h"
#include "utils.h"
#include "string_tokenizer.h"

using namespace htmlcxx;
using namespace std;

void LinksProcessor::process() {
  HTML::ParserDom parser;

  while (document_source_->fetchNext()) {
    string url = document_source_->getUrl();
    int_id docId = docDb_->getId(url);
    DocDbEntry& docEntry = docDb_->get(docId);

    string html = document_source_->getText();
    int charset = document_source_->getCharset();
    
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    int outlinks = 0;

    for (; it != end; ++it) {
      
      if ((!it->isTag()) && (!it->isComment())) {
        DocDbEntry& otherDocEntry = docEntry;

        tree<HTML::Node>::iterator parentTag = dom.parent(it);
        string tagName = parentTag->tagName();
        lowerString(tagName);
        
        if (tagName == "a") {
          parentTag->parseAttributes();
          pair<bool, string> hrefAttribute = parentTag->attribute("href");
          if (hrefAttribute.first) {
            string href = hrefAttribute.second;
            if (href != url) {
              outlinks++;
              int_id otherDoc = docDb_->getId(href);
              if (otherDoc > 0) {
                otherDocEntry = docDb_->get(otherDoc);
                otherDocEntry.addBackref(docId);
                cout << "backref: " << url << " -> " << hrefAttribute.second << endl;
              }

            }
          }
        } else if (tagName == "title") {
          //
        } else {
          continue;
        }
        
        string input = it->text();
        StringTokenizer* st = stringTokenizer(&input, charset);
        string term;
        while (st->fetch(term)) {
          unordered_map<string, VocabularyEntry>::const_iterator it = vocabulary_->find(term);
          if (it == vocabulary_->end()) {
            cout << "term not found: " << term << endl;
          }
          otherDocEntry.addTerm(it->second.id_);
        }
        delete st;
      }
      
    }

    docEntry.outlinks = outlinks;
    // danilo
  }

}

