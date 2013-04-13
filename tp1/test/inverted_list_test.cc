#include <string>
#include "gtest/gtest.h"
#include "utils.h"
#include "vocabulary.h"
#include "inverted_list.h"

using namespace std;

TEST(InvertedList, shouldFindNoDocs) {
  Vocabulary voc;
  InvertedListWriter* writer = createInvertedListWriter(voc, "t1");
  writer->close();
  delete writer;

  InvertedList* reader = openInvertedList("t1");
  ASSERT_EQ((unsigned int) 0, reader->countDocs("danilo"));
  delete reader;
}

TEST(InvertedList, shouldFindOneDoc) {
  Vocabulary voc;
  voc.put("danilo");
  voc.increment("danilo");
  InvertedListWriter* writer = createInvertedListWriter(voc, "t2");
  writer->writeEntry(1, 1, 1);
  writer->close();
  delete writer;

  InvertedList* reader = openInvertedList("t2");
  ASSERT_EQ((unsigned int) 1, reader->countDocs("danilo"));
  delete reader;
}

TEST(InvertedList, shouldFindDocs) {
  Vocabulary voc;
  voc.put("danilo");
  voc.setDf("danilo", 2);
  voc.put("ferreira");
  voc.setDf("ferreira", 1);
  InvertedListWriter* writer = createInvertedListWriter(voc, "t3");
  writer->writeEntry(1, 1, 1);
  writer->writeEntry(1, 2, 3);
  writer->writeEntry(2, 1, 1);
  writer->close();
  delete writer;

  InvertedList* reader = openInvertedList("t3");
  ASSERT_EQ((unsigned int) 2, reader->countDocs("danilo"));

  vector<TermOccurrence> ocurrences;
  reader->findDocs("danilo", ocurrences);
  ASSERT_EQ((unsigned int) 2, ocurrences.size());

  ASSERT_EQ((unsigned int) 1, ocurrences[0].doc_);
  ASSERT_EQ((unsigned int) 1, ocurrences[0].tf_);

  ASSERT_EQ((unsigned int) 2, ocurrences[1].doc_);
  ASSERT_EQ((unsigned int) 3, ocurrences[1].tf_);
  
  ASSERT_EQ((unsigned int) 1, reader->countDocs("ferreira"));
  delete reader;
}
