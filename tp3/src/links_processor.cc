#include <iostream>
#include <sstream>
#include <htmlcxx/html/ParserDom.h>
#include "links_processor.h"
#include "utils.h"
#include "string_tokenizer.h"

using namespace htmlcxx;
using namespace std;

void LinksProcessor::process(DocDb& docDb) {
  HTML::ParserDom parser;

  size_t totalLinks = 0;
  size_t totalValidLinks = 0;

  while (document_source_->fetchNext()) {
    string url = document_source_->getUrl();
    int_id docId = docDb.getId(url);
    if (docId == 0) {
      cout << "ERROR doc not found: " << url << endl;
      continue;
    }
    //DocDbEntry& docEntry = docDb.get(docId);

    string html = document_source_->getText();
    int charset = document_source_->getCharset();
    
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    for (; it != end; ++it) {
      
      if ((!it->isTag()) && (!it->isComment())) {
        //DocDbEntry& otherDocEntry = docEntry;

        tree<HTML::Node>::iterator parentTag = dom.parent(it);
        string tagName = parentTag->tagName();
        lowerString(tagName);
        
        if (tagName == "a") {
          parentTag->parseAttributes();
          pair<bool, string> hrefAttribute = parentTag->attribute("href");
          if (hrefAttribute.first) {
            string href = hrefAttribute.second;

            if (href.compare(0, 4, "http") == 0 && href != url) {
              //cout << "ref: " << " -> " << href << endl;

              totalLinks++;
              
              int_id otherDoc = docDb.getId(href);
              if (otherDoc > 0) {
                //otherDocEntry = docDb.get(otherDoc);
                //docDb.linkDocuments(docId, otherDoc);
                totalValidLinks++;

                string input = it->text();
                emitTerms(input, charset, otherDoc);

                //cout << "backref: " << input << " -> " << href << endl;
              }
            }
          }
        } else if (tagName == "title") {
          string input = it->text();
          emitTerms(input, charset, docId);
        }

      }
      
    }

  }

  cout << totalLinks << " total links found" << endl;
  //cout << totalValidLinks << " links inside collection" << endl;

}

void LinksProcessor::emitTerms(const string& text, int charset, int_id docId) {
  StringTokenizer* st = stringTokenizer(&text, charset);
  string term;
  //cout << "doc " << docId << ": ";
  while (st->fetch(term)) {
    unordered_map<string, VocabularyEntry>::const_iterator it = vocabulary_->find(term);
    if (it == vocabulary_->end()) {
      cout << "term not found: " << term << endl;
    } else {
      //cout << term << " ";
    }
    int_id termId = it->second.id_;

    if (atf_->at(termId - 1).count(docId)) {
      atf_->at(termId - 1)[docId] += 1;
    } else {
      atf_->at(termId - 1)[docId] = 1;
    }
  }
  //cout << endl;
  delete st;
}