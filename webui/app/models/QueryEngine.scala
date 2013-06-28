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
  
  def bm25a(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75, anchorBoost: Double): QueryEngine = {
    new Bm25AQueryEngine(vocabulary, documentDb, k1, b, anchorBoost)
  }
  
  def bm25AOnly(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75): QueryEngine = {
    new Bm25AOnlyQueryEngine(vocabulary, documentDb, k1, b)
  }

  def bm25pra(vocabulary: Vocabulary, documentDb: DocumentDb, k1: Double = 1.0, b: Double = 0.75, anchorBoost: Double): QueryEngine = {
    new Bm25praQueryEngine(vocabulary, documentDb, k1, b, anchorBoost)
  }
  
}

trait PageRankContribution {
  def pageRankContribution(acumulators: AccumulatorPool, docId: Int, doc: Document): Double
}

trait NoPagerank extends PageRankContribution {
  def pageRankContribution(acumulators: AccumulatorPool, docId: Int, doc: Document): Double = 1.0
}

trait PagerankFactor extends PageRankContribution {
  def pageRankContribution(acumulators: AccumulatorPool, docId: Int, doc: Document): Double = {
    val factor = doc.pr
    acumulators.multiplyAccumulatorBy(docId, factor)
    factor
  }
}

trait LogPagerankFactor extends PageRankContribution {
  def pageRankContribution(acumulators: AccumulatorPool, docId: Int, doc: Document): Double = {
    val factor = if (doc.indegree > 50) {
      1.0 + Math.log(0.85 + doc.pr)
    } else {
      1.0
    }
    acumulators.multiplyAccumulatorBy(docId, factor)
    factor
  }
}

abstract class AbstractQueryEngine(
  val vocabulary: Vocabulary,
  val documentDb: DocumentDb) extends QueryEngine with PageRankContribution {

  def findDocuments(query: String, r: Int, accumulators: Int): List[QueryResult] = {

    val termEntries = new ListBuffer[TermEntry]

    val normalized = StringUtils.normalize(query)
    for (term <- normalized.split(" ")) {
      val termEntry = vocabulary.findTerm(term)
      if (termEntry.bodyFreq > 0) {
        termEntries += termEntry
      }
    }

    var maxPr = 0.0;

    termEntries.sortWith(
      (t1, t2) => t1.bodyFreq < t2.bodyFreq)

    val accumulatorPool = new AccumulatorPool(accumulators)

    for (termEntry <- termEntries) {
      val idf1 = Math.log(1 + (documentDb.count.toDouble / termEntry.bodyFreq))
      val idf2 = Math.log(((documentDb.count.toDouble - termEntry.bodyFreq + 0.5) / (termEntry.bodyFreq + 0.5)))

      // Ocorrências no corpo
      if (bodyTermsBoost > 0.0) {
        val bodyCursor = vocabulary.findOccurences(termEntry)
        try {
          while (bodyCursor.fetchNext) {
            val doc = documentDb.get(bodyCursor.documentId)
            maxPr = Math.max(maxPr, doc.pr)
            val termDocWeight = bodyTermsBoost * computeTermContribution(bodyCursor.freq, idf1, idf2, doc)
            accumulatorPool.addToAccumulator(bodyCursor.documentId, termDocWeight)
          }
        } finally {
          bodyCursor.close
        }

      }

      // Ocorrências em anchor text
      if (anchorTermsBoost > 0.0) {

        val anchorCursor = vocabulary.findAnchorOccurences(termEntry)
        try {
          while (anchorCursor.fetchNext) {
            val doc = documentDb.get(anchorCursor.documentId)
            //maxPr = Math.max(maxPr, doc.pr)
            val termDocWeight = anchorTermsBoost * computeTermContribution(anchorCursor.freq, idf1, idf2, doc)
            accumulatorPool.addToAccumulator(anchorCursor.documentId, termDocWeight)
          }
        } finally {
          anchorCursor.close
        }
      }

    }

    val iter = accumulatorPool.keyIterator

    val heap = new HeapList[QueryResult](Math.min(accumulatorPool.size(), r))

    //val uniq = new HashSet[String]

    while (iter.hasNext) {
      {
        val docId = iter.next
        val doc = documentDb.get(docId)

        val normalizationFactor = computeNormalizationFactor(doc)
        val pr = pageRankContribution(accumulatorPool, docId, doc)

        accumulatorPool.multiplyAccumulatorBy(docId, normalizationFactor)
        //val prVal = prf * (doc.pr / maxPr)
        //val prVal = prf * doc.pr

        heap.pushAndReplaceMin(new QueryResult(doc.url, accumulatorPool.getValue(docId), pr))

      }

    }

    val result: MutableList[QueryResult] = MutableList()
    while (heap.size() > 0) {
      heap.popMin() +=: result
    }

    result.toList
  }

  def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document): Double

  def computeNormalizationFactor(doc: Document): Double

  val bodyTermsBoost = 1.0

  val anchorTermsBoost = 0.0

}

class VectorQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb) extends AbstractQueryEngine(vocabulary, documentDb) with NoPagerank {

  override def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document) = {
    (1 + Math.log(freqInDoc)) * idf1
  }

  def computeNormalizationFactor(doc: Document) = {
    1.0 / doc.norm
  }

  def name = "vector"
}

abstract class AbstractBm25QueryEngine(
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
  override val b: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with NoPagerank {

  override def computeTermContribution(freqInDoc: Int, idf1: Double, idf2: Double, doc: Document) = {
    val vector = ((1 + Math.log(freqInDoc)) * idf1) / doc.norm
    val bm25 = super.computeTermContribution(freqInDoc, idf1, idf2, doc)
    ((1.0 - c) * bm25) + (c * vector)
  }

  override def name = "vbm[c=%.1f;k1=%.1f;b=%.1f]".format(c, k1, b)
}

class Bm25QueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with NoPagerank {

  override def name = "bm25".format(k1, b)
}

class Bm25AOnlyQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with NoPagerank {

  override def name = "bm25-anchorOnly".format(k1, b)
  
  override val anchorTermsBoost = 1.0
  override val bodyTermsBoost = 0.0
}

class Bm25AQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double,
  override val bodyTermsBoost: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with NoPagerank {

  override def name = "bm25-anchor".format(k1, b)
  
  override val anchorTermsBoost = 1.0
}

class Bm25PrQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with PagerankFactor {

  override def name = "bm25pr[k1=%.1f;b=%.1f]".format(k1, b)
}

class Bm25LogPrQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with LogPagerankFactor {

  override def name = "bm25logpr[k1=%.1f;b=%.1f]".format(k1, b)
}

class Bm25praQueryEngine(
  override val vocabulary: Vocabulary,
  override val documentDb: DocumentDb,
  override val k1: Double,
  override val b: Double,
  override val anchorTermsBoost: Double) extends AbstractBm25QueryEngine(vocabulary, documentDb, k1, b) with LogPagerankFactor {

  override def name = "bm25-anchor-pr"
  override val bodyTermsBoost = 1.0
}
