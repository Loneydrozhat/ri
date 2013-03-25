#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include "term_extractor.h"

using namespace std;
using namespace htmlcxx;

class HtmlTermExtractor : public TermExtractor {
  private:
    tree<HTML::Node>::iterator it_;
    tree<HTML::Node>::iterator end_;

    bool findTextNode() {
      for (; it_ != end_; ++it_) {
        if ((!it_->isTag()) && (!it_->isComment()) && !(it_->text().empty())) {
          return true;
        }
      }
      return false;
    }

  public:
    HtmlTermExtractor(const string &html) {
      HTML::ParserDom parser;
      tree<HTML::Node> dom = parser.parseTree(html);
      
      //Dump all links in the tree
      it_ = dom.begin();
      end_ = dom.end();
      cout << "has: " << (it_ != end_) << endl;

      cout << "construtor" << endl;
      cout << dom << endl;
    }

    bool hasNext() {
      return findTextNode();
    }

    string next() {
      return it_->text();
    }
};

TermExtractor* htmlExtractor(const string &html) {
  return new HtmlTermExtractor(html);
}
