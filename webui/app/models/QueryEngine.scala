package models

import java.io.File

import scala.collection.mutable.ListBuffer

import utils.StringUtils

trait QueryEngine {
  val vocabulary: Vocabulary
  def findDocuments(rankingModel: RankingModel, query: String, k: Int): List[QueryResult]
}

object QueryEngine {
  def init(vocabularyFile: File, indexFile: File, documentsFile: File): QueryEngine = {

    val vocabulary = Vocabulary.fromFile(vocabularyFile, indexFile)
    val documentDb = DocumentDb.fromFile(documentsFile)

    new QueryEngineImpl(vocabulary, documentDb)
  }
}

class QueryEngineImpl(
  val vocabulary: Vocabulary,
  private val documentDb: DocumentDb) extends QueryEngine {

  def findDocuments(rankingModel: RankingModel, query: String, k: Int) = {

    val termEntries = new ListBuffer[TermEntry]

    val normalized = StringUtils.normalize(query)
    for (term <- normalized.split(" ")) {
      val termEntry = vocabulary.findTerm(term)
      if (termEntry.totalFreq > 0) {
        termEntries += termEntry
      }
    }

    termEntries.sortWith(
      (t1, t2) => t1.totalFreq < t2.totalFreq)

    val result = new ListBuffer[QueryResult]

    for (termEntry <- termEntries) {
      val cursor = termEntry.findOccurences
      try {
        while (cursor.fetchNext) {
          val doc = documentDb.get(cursor.documentId)
          result += QueryResult(doc.url, 0.5)
        }

      } finally {
        cursor.close
      }
    }

    result.toList
  }

}