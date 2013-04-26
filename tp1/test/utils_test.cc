#include <string>
#include "gtest/gtest.h"
#include "utils.h"
#include "file_handler.h"
#include "heap.h"
#include "string_tokenizer.h"

using namespace std;

TEST(stripHttpHeaders, shouldPreserveOriginal) {
  string original = "some random stuff.";
  string result = stripHttpHeaders(original);
  ASSERT_EQ(original, result);
}

TEST(stripHttpHeaders, shouldStripHeaders) {
  string original = 
    "HTTP/1.1 200 OK\n"
    "Date: Mon, 09 Mar 2009 20:49:35 GMT\n"
    "Server: Apache/1.3.41 (Unix) PHP/5.2.3 mod_jk/1.2.25 mod_log_bytes/1.2 mod_bwlimited/1.4 mod_auth_passthrough/1.8 FrontPage/5.0.2.2635 mod_ssl/2.8.31 OpenSSL/0.9.7a\n"
    "X-Powered-By: PHP/5.2.3\n"
    "Connection: close\n"
    "Content-Type: text/html\n"
    "\n"
    "<html><body>content</body></html>";

  string result = stripHttpHeaders(original);
  ASSERT_EQ("<html><body>content</body></html>", result);
}


TEST(FileHandler, shouldWriteAndReadInts) {
  FileHandler* tempf = createFile("b1.tmp");
  tempf->writeInt(1);
  tempf->writeInt(2);
  tempf->writeInt(3000000000);

  tempf->setSeekPos(0);

  ASSERT_EQ((int_id) 1, tempf->readInt());
  ASSERT_EQ((int_id) 2, tempf->readInt());
  ASSERT_EQ((int_id) 3000000000, tempf->readInt());

  ASSERT_EQ((ifstream::pos_type) 12, tempf->getSeekPos());

  delete tempf;
}

TEST(FileHandler, shouldWriteAndReadStrings) {
  FileHandler* tempf = createFile("b2.tmp");
  tempf->writeString("foo");
  tempf->writeString("bar");
  tempf->writeString("baz");

  tempf->setSeekPos(0);

  ASSERT_EQ("foo", tempf->readString());
  ASSERT_EQ("bar", tempf->readString());
  ASSERT_EQ("baz", tempf->readString());

  ASSERT_EQ((ifstream::pos_type) 12, tempf->getSeekPos());

  delete tempf;
}

TEST(heap, shouldPopMin) {
  int array[] = {4, 1, 7, 3, 0, 6, 2, 5};
  vector<int> vec (array, array + sizeof(array)/sizeof(int));

  buildMinHeap(vec);

  ASSERT_EQ(0, popMin(vec));
  ASSERT_EQ(1, popMin(vec));
  ASSERT_EQ(2, popMin(vec));
  ASSERT_EQ(3, popMin(vec));
  ASSERT_EQ(4, popMin(vec));
  ASSERT_EQ(5, popMin(vec));
  ASSERT_EQ(6, popMin(vec));
  ASSERT_EQ(7, popMin(vec));

  ASSERT_EQ((unsigned int) 0, vec.size());
}

TEST(heap, shouldPopMinAndPush) {
  int array[] = {4, 1, 7, 3, 0, 6, 2, 5};
  vector<int> vec (array, array + sizeof(array)/sizeof(int));

  buildMinHeap(vec);

  ASSERT_EQ(0, popMinAndPush(vec, 8));
  ASSERT_EQ((unsigned int) 8, vec.size());
  ASSERT_EQ(1, popMin(vec));
  ASSERT_EQ((unsigned int) 7, vec.size());
  ASSERT_EQ(2, popMinAndPush(vec, 0));
  ASSERT_EQ(0, popMinAndPush(vec, 9));
  ASSERT_EQ((unsigned int) 7, vec.size());  
}

TEST(identifyCharset, shouldReturnUtf8) {
  ASSERT_EQ(UTF_8, identifyCharset("charset=UTF-8"));
  ASSERT_EQ(UTF_8, identifyCharset("charset=utf-8"));
  ASSERT_EQ(UTF_8, identifyCharset("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"));
}

TEST(identifyCharset, shouldReturnIso) {
  ASSERT_EQ(ISO_8859_1, identifyCharset("charset=ISO-8859-1"));
  ASSERT_EQ(ISO_8859_1, identifyCharset("charset=iso-8859-1"));
  ASSERT_EQ(ISO_8859_1, identifyCharset("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">"));
}

TEST(identifyCharset, shouldReturnUnknown) {
  ASSERT_EQ(UNKNOWN, identifyCharset("some random text"));
  ASSERT_EQ(UNKNOWN, identifyCharset("iso-8859-1"));
  ASSERT_EQ(UNKNOWN, identifyCharset("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=other\">"));
}


TEST(StringTokenizer, doNothingWithEmptyString) {
  string input = "";
  StringTokenizer* st = stringTokenizer(&input, UTF_8);
  string term;
  ASSERT_FALSE(st->fetch(term));
  delete st;
}

TEST(StringTokenizer, fetchSingleTerm) {
  string input = "danilo";
  StringTokenizer* st = stringTokenizer(&input, UTF_8);
  string term;
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("danilo", term);
  ASSERT_FALSE(st->fetch(term));
  delete st;
}

TEST(StringTokenizer, fetchMultipleTerms) {
  string input = "t1 t2";
  StringTokenizer* st = stringTokenizer(&input, UTF_8);
  string term;
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("t1", term);
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("t2", term);
  ASSERT_FALSE(st->fetch(term));
  delete st;
}

TEST(StringTokenizer, lowerTerms) {
  string input = "Danilo";
  StringTokenizer* st = stringTokenizer(&input, UTF_8);
  string term;
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("danilo", term);
  delete st;
}

TEST(StringTokenizer, replaceHtmlEntities) {
  string input = "Danilo&nbsp;Ferr&amp;";
  StringTokenizer* st = stringTokenizer(&input, UTF_8);
  string term;
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("danilo", term);
  ASSERT_TRUE(st->fetch(term));
  ASSERT_EQ("ferr", term);
  delete st;
}
