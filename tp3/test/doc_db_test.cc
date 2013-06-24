#include <string>
#include "gtest/gtest.h"
#include "doc_db.h"

using namespace std;

TEST(docDb, shouldStoreData) {
  DocDb docDb;

  docDb.add("http://a.b.c/1");
  docDb.add("http://a.b.c/2");
  docDb.add("http://a.b.c/3");
  docDb.add("http://a.b.c/4");

  ASSERT_EQ((unsigned int) 1, docDb.getId("http://a.b.c/1"));
  ASSERT_EQ((unsigned int) 2, docDb.getId("http://a.b.c/2"));
  ASSERT_EQ((unsigned int) 3, docDb.getId("http://a.b.c/3"));
  ASSERT_EQ((unsigned int) 4, docDb.getId("http://a.b.c/4"));

  DocDbEntry& entry = docDb.get(3);
  entry.addBackref(2);
  entry.addBackref(4);

  ASSERT_EQ((unsigned int) 2, entry.backrefsCount());

  DocDbEntry& entry2 = docDb.get(3);
  ASSERT_EQ((unsigned int) 2, entry2.backrefsCount());

  entry2.addTerm(1);
  entry2.addTerm(7);
  entry2.addTerm(2);
  entry2.addTerm(2);
  entry2.addTerm(7);
  entry2.addTerm(3);

  vector<int_id>& terms = entry2.getTerms();
  ASSERT_EQ((unsigned int) 4, terms.size());

  ASSERT_EQ((unsigned int) 0, docDb.getId("http://a.b.c/notfound"));

}
