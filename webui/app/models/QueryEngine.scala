package models

import scala.collection.mutable.ListBuffer
import scala.collection.mutable.MutableList

import utils.AccumulatorPool
import utils.HeapList
import utils.StringUtils

trait QueryEngine {
  val vocabulary: Vocabulary
  val documentDb: DocumentDb
  def findDocuments(query: String, r: Int, accumulators: Int): List[QueryResult]
}

object QueryEngine {
  def vector(vocabulary: Vocabulary, documentDb: DocumentDb): QueryEngine = {
    new VectorQueryEngine(vocabulary, documentDb)
  }
  
  def bm25(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75): QueryEngine = {
    new Bm25QueryEngine(vocabulary, documentDb, k1, b)
  }

  def vectorBm25(vocabulary: Vocabulary, documentDb: DocumentDb, c: Double, k1: Double = 1.0, b: Double = 0.75): QueryEngine = {
    new VectorBm25QueryEngine(vocabulary, documentDb, c, k1, b)
  }
}

abstract class AbstractQueryEngine(
  val vocabulary: Vocabulary,
  val documentDb: DocumentDb) extends QueryEngine {

  def findDocuments(query: String, r: Int, accumulators: Int): List[QueryResult] = {

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

    val accumulatorPool = new AccumulatorPool(accumulators)

    for (termEntry <- termEntries) {
      val cursor = termEntry.findOccurences
      val idf1 = Math.log(1 + (documentDb.count.toDouble / termEntry.totalFreq))
      val idf2 = Math.log(((documentDb.count.toDouble - termEntry.totalFreq + 0.5) / (termEntry.totalFreq + 0.5)))
      try {
        while (cursor.fetchNext) {
          val doc = documentDb.get(cursor.documentId)
          val termDocWeight = computeTermContribution(cursor.freq, idf1, idf2, doc)
          accumulatorPool.addToAccumulator(cursor.documentId, termDocWeight)
        }
      } finally {
        cursor.close
      }
    }

    val iter = accumulatorPool.keyIterator

    val heap = new HeapList[QueryResult](Math.min(accumulatorPool.size(), r))

    while (iter.hasNext) {
      val docId = iter.next
      val doc = documentDb.get(docId)
      
      val normalizationFactor = computeNormalizationFactor(doc)
      accumulatorPool.multiplyAccumulatorBy(docId, normalizationFactor)
      heap.pushAndReplaceMin(new QueryResult(doc.url, accumulatorPool.getValue(docId)))
    }

    val result: MutableList[QueryResult] = MutableList()
    while (heap.size() > 0) {
      heap.popMin() +=: result
    }

    result.toList
  }

  def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document): Double
  
  def computeNormalizationFactor(doc: Document): Double

}

class VectorQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb) extends AbstractQueryEngine(vocabulary, documentDb) {

  override def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document) = {
    (1 + Math.log(freqInDoc)) * idf1
  }
  
  def computeNormalizationFactor(doc: Document) = {
    1.0 / doc.norm
  }
}

class Bm25QueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  val k1: Double,
  val b: Double) extends AbstractQueryEngine(vocabulary, documentDb) {

  override def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document) = {
    val numerator = (k1 + 1.0) * freqInDoc
    val lengthFactor = doc.length / documentDb.avgLength
    val denominator = (k1 * (1.0 - b + (b * lengthFactor))) + freqInDoc
    idf2 * (numerator / denominator) 
  }
  
  def computeNormalizationFactor(doc: Document) = {
    1.0
  }
}


class VectorBm25QueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  val c: Double,
  override val k1: Double,
  override val b: Double) extends Bm25QueryEngine(vocabulary, documentDb, k1, b) {

  override def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document) = {
    val vector = ((1 + Math.log(freqInDoc)) * idf1) / doc.norm
    val bm25 = super.computeTermContribution(freqInDoc, idf1, idf2, doc)
    ((1.0 - c) * bm25) + (c * vector) 
  }

}

