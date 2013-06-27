#include <string>
#include "gtest/gtest.h"
#include "doc_db.h"
#include "page_ranker.h"

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

  docDb.linkDocuments(3, 1);
  docDb.linkDocuments(3, 2);
  docDb.linkDocuments(3, 4);


  DocDbEntry& entry = docDb.get(3);
  ASSERT_EQ((unsigned int) 3, entry.outDegree());

  docDb.linkDocuments(1, 4);
  docDb.linkDocuments(2, 4);

  DocDbEntry& entry2 = docDb.get(4);
  ASSERT_EQ((unsigned int) 3, entry2.inDegree());

  entry2.addTerm(1);
  entry2.addTerm(7);
  entry2.addTerm(2);
  entry2.addTerm(2);
  entry2.addTerm(7);
  entry2.addTerm(3);

  ASSERT_EQ((unsigned int) 4, entry2.termsCount());

  ASSERT_EQ((unsigned int) 0, docDb.getId("http://a.b.c/notfound"));

  unsigned int maxInDegree = 0;
  unsigned int inDegreeSum = 0;
  unsigned int outDegreeSum = 0;
  unsigned int inDegreeCount = 0;
  for (unsigned int i = 0, n = docDb.size(); i < n; i++) {
    DocDbEntry& docEntry = docDb.get(i + 1);
    size_t inlinkCount = docEntry.inDegree();
    maxInDegree = (inlinkCount > maxInDegree) ? inlinkCount : maxInDegree;
    inDegreeSum += inlinkCount;
    outDegreeSum += docEntry.outDegree();
    if (inlinkCount > 0) {
      inDegreeCount++;
    }
  }

  ASSERT_EQ((unsigned int) 4, docDb.size());
  ASSERT_EQ((unsigned int) 3, maxInDegree);
  ASSERT_EQ((unsigned int) 5, inDegreeSum);
  ASSERT_EQ((unsigned int) 5, outDegreeSum);
  ASSERT_EQ((unsigned int) 3, inDegreeCount);

}


TEST(pageRanker, shouldComputePr) {
  DocDb docDb;
  docDb.add("http://1");
  docDb.add("http://2");
  docDb.add("http://3");
  docDb.linkDocuments(1, 2);
  docDb.linkDocuments(1, 3);
  docDb.linkDocuments(2, 3);
  docDb.linkDocuments(3, 1);

  PageRanker pageRanker(&docDb, 0.5);

  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(1));
  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(2));
  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(3));

  pageRanker.computeIteration();

  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(1));
  ASSERT_DOUBLE_EQ(0.75, pageRanker.getPr(2));
  ASSERT_DOUBLE_EQ(1.25, pageRanker.getPr(3));

  for (int i = 0; i < 50; i++) {
    pageRanker.computeIteration();
  }

  ASSERT_NEAR(1.07692308, pageRanker.getPr(1), 0.00001);
  ASSERT_NEAR(0.76923077, pageRanker.getPr(2), 0.00001);
  ASSERT_NEAR(1.15384615, pageRanker.getPr(3), 0.00001);
}

TEST(pageRanker, shouldNotExplode) {
  DocDb docDb;
  for (int i = 0; i < 54; i++) {
    docDb.add("http://X");
  }
  
  docDb.add("http://1");
  docDb.add("http://2");
  docDb.add("http://3");
  docDb.add("http://4");
  docDb.add("http://5");
  docDb.add("http://6");


  docDb.linkDocuments(docDb.getId("http://1"), docDb.getId("http://2"));
  docDb.linkDocuments(docDb.getId("http://2"), docDb.getId("http://3"));
  docDb.linkDocuments(docDb.getId("http://3"), docDb.getId("http://1"));
  docDb.linkDocuments(docDb.getId("http://1"), docDb.getId("http://4"));
  docDb.linkDocuments(docDb.getId("http://2"), docDb.getId("http://4"));
  docDb.linkDocuments(docDb.getId("http://3"), docDb.getId("http://4"));

  PageRanker pageRanker(&docDb, 0.15);

  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(docDb.getId("http://1")));
  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(docDb.getId("http://2")));
  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(docDb.getId("http://3")));
  ASSERT_DOUBLE_EQ(1.0, pageRanker.getPr(docDb.getId("http://4")));

  for (int i = 0; i < 100; i++) {
    pageRanker.computeIteration();
  }

  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://1")), 0.00001);
  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://2")), 0.00001);
  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://3")), 0.00001);
  ASSERT_NEAR(0.482608695, pageRanker.getPr(docDb.getId("http://4")), 0.00001);
  ASSERT_NEAR(0.15, pageRanker.getPr(docDb.getId("http://5")), 0.00001);
  ASSERT_NEAR(0.15, pageRanker.getPr(docDb.getId("http://6")), 0.00001);

  for (int i = 0; i < 500; i++) {
    pageRanker.computeIteration();
  }

  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://1")), 0.00001);
  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://2")), 0.00001);
  ASSERT_NEAR(0.260869565, pageRanker.getPr(docDb.getId("http://3")), 0.00001);
  ASSERT_NEAR(0.482608695, pageRanker.getPr(docDb.getId("http://4")), 0.00001);
  ASSERT_NEAR(0.15, pageRanker.getPr(docDb.getId("http://5")), 0.00001);
  ASSERT_NEAR(0.15, pageRanker.getPr(docDb.getId("http://6")), 0.00001);
}
