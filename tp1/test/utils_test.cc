#include <string>
#include "gtest/gtest.h"
#include "utils.h"
#include "temp_file.h"
#include "heap.h"

using namespace std;

TEST(stripHttpHeaders, preserveOriginal) {
  string original = "some random stuff.";
  string result = stripHttpHeaders(original);
  ASSERT_EQ(original, result);
}

TEST(stripHttpHeaders, stripHeaders) {
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

TEST(stripSpecialChars, removePontuationFromSuffix) {
  ASSERT_EQ("danilo", stripSpecialChars("danilo!"));
  ASSERT_EQ("danilo", stripSpecialChars("danilo.?;:"));
}

TEST(stripSpecialChars, removePontuationFromPrefix) {
  ASSERT_EQ("danilo", stripSpecialChars(":danilo"));
  ASSERT_EQ("danilo", stripSpecialChars("([{danilo"));
}

TEST(stripSpecialChars, stripNonVisible) {
  ASSERT_EQ("danilo", stripSpecialChars("\t danilo"));
  ASSERT_EQ("danilo", stripSpecialChars(" danilo\r\n "));
}

TEST(TempFile, createTempFile) {
  TempFile* tempf = tempFile();
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

TEST(TempFile, sizeOf) {
  ASSERT_EQ((unsigned int) 4, sizeof(int_id));
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
