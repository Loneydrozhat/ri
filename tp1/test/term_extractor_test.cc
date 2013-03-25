#include <string>
#include <htmlcxx/html/ParserDom.h>
#include "gtest/gtest.h"
#include "term_extractor.h"

using namespace std;

void expectTerms(string &input, string expected[], int termsCount) {
  TermExtractor* extractor = htmlExtractor(input);
  int i = 0;
  for (; extractor->hasNext(); i++) {
    string term = extractor->next();
    EXPECT_EQ(expected[i], term);
  }
  EXPECT_EQ(termsCount, i);
  delete extractor;
}

TEST(TermExtractorTest, shouldExtractTerms) {
  string input("<html><body>hey</body></html>");
  string expected[] = {"hey"};
  expectTerms(input, expected, 1);
}

/*
TEST(TermExtractorTest, shouldParseSomeHtml) {

  //Parse some html code
  string html = "<html><body>hey</body></html>";
  HTML::ParserDom parser;
  tree<HTML::Node> dom = parser.parseTree(html);
  
  //Print whole DOM tree
  cout << dom << endl;
  
  //Dump all links in the tree
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it) {
     if (strcasecmp(it->tagName().c_str(), "A") == 0) {
       it->parseAttributes();
       cout << it->attribute("href").second << endl;
     }
  }
  
  //Dump all text of the document
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it) {
    if ((!it->isTag()) && (!it->isComment())) {
      cout << it->text();
    }
  }
  cout << endl;

  EXPECT_EQ("Hello, world!", "Hello, world!");
}
*/