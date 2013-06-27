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
  def name: String
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
  
  def bm25Pr(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75): QueryEngine = {
    new Bm25PrQueryEngine(vocabulary, documentDb, k1, b)
  }
  
  def bm25LogPr(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75): QueryEngine = {
    new Bm25LogPrQueryEngine(vocabulary, documentDb, k1, b)
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

    var maxPr = 0.0;
    
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
          maxPr = Math.max(maxPr, doc.pr)
          val termDocWeight = computeTermContribution(cursor.freq, idf1, idf2, doc)
          accumulatorPool.addToAccumulator(cursor.documentId, termDocWeight)
        }
      } finally {
        cursor.close
      }
    }

    val iter = accumulatorPool.keyIterator

    val heap = new HeapList[QueryResult](Math.min(accumulatorPool.size(), r))

    //val uniq = new HashSet[String]

    while (iter.hasNext) { {
        val docId = iter.next
        val doc = documentDb.get(docId)
        
        val normalizationFactor = computeNormalizationFactor(doc)
        accumulatorPool.multiplyAccumulatorBy(docId, normalizationFactor)
        //val prVal = prf * (doc.pr / maxPr)
        //val prVal = prf * doc.pr
        
        //accumulatorPool.addToAccumulator(docId, prVal);
        //accumulatorPool.multiplyAccumulatorBy(docId, prVal)
        
        heap.pushAndReplaceMin(new QueryResult(doc.url, accumulatorPool.getValue(docId), doc.pr))
        
//      if (uniq.contains(doc.url)) {
//        Console.println(doc.url);
//      } else {
//        uniq.add(doc.url)
//      }
      }
      
//      if (uniq.contains(doc.url)) {
//        Console.println(doc.url);
//      } else {
//        uniq.add(doc.url)
//      }
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
  
  def name = "vector"
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
  
  def name = "bm25[k1=%.1f;b=%.1f]".format(k1, b)
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

  override def name = "vbm[c=%.1f;k1=%.1f;b=%.1f]".format(c, k1, b)
}

class Bm25PrQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends Bm25QueryEngine(vocabulary, documentDb, k1, b) {

  override def computeNormalizationFactor(doc: Document) = {
    doc.pr
  }
  
  override def name = "bm25pr[k1=%.1f;b=%.1f]".format(k1, b)
}

class Bm25LogPrQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends Bm25QueryEngine(vocabulary, documentDb, k1, b) {

  override def computeNormalizationFactor(doc: Document) = {
    1.0 + Math.log(doc.pr)
  }
  
  override def name = "bm25logpr[k1=%.1f;b=%.1f]".format(k1, b)
}
