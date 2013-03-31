#include <string>
#include "gtest/gtest.h"
#include "utils.h"

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
