package models

import java.io.BufferedInputStream
import java.io.DataInputStream
import java.io.File
import java.io.FileInputStream
import java.util.HashMap
import utils.FileUtils
import scala.collection.Iterator

trait Vocabulary extends Iterable[TermEntry] {
  def findTerm(term: String): TermEntry
  def iterator: Iterator[TermEntry]
  def findOccurences(termEntry: TermEntry): OccurencesCursor
  def findAnchorOccurences(termEntry: TermEntry): OccurencesCursor
}

class TermEntry(
  val term: String,
  var bodyFreq: Int,
  var bodyPointer: Long,
  var anchorFreq: Int,
  var anchorPointer: Long
  ) {
}

trait OccurencesCursor {
  def fetchNext: Boolean
  def documentId: Int
  def freq: Int
  def close: Unit = {}
}

object NilCursor extends OccurencesCursor {
  def fetchNext = false
  def documentId = 0
  def freq = 0
}

class OccurencesCursorImpl(val size: Int, indexFile: File, pointer: Long) extends OccurencesCursor {

  private val is = new DataInputStream(new BufferedInputStream(new FileInputStream(indexFile)))
  val skipped = is.skip(pointer)
  if (skipped != pointer) {
    throw new RuntimeException("invalid file format")
  }

  var i = 0;
  var documentId_ : Int = 0;
  var freq_ : Int = 0;

  def fetchNext: Boolean = {
    if (i >= size) {
      false
    } else {
      documentId_ = FileUtils.readBleInt(is, 4)
      freq_ = FileUtils.readBleInt(is, 4)
//      Console.printf("(%d, %d)", documentId_, freq_)
      i += 1
      true
    }
  }
  override def close() {
    is.close()
  }
  def documentId = documentId_
  def freq = freq_
}

object Vocabulary {
  def fromFile(vocabularyFile: File, indexFile: File, avocabularyFile: File, aindexFile: File): Vocabulary = {

    val hmap = new HashMap[String, TermEntry]

	val inputStream = new DataInputStream(new BufferedInputStream(new FileInputStream(vocabularyFile)))
    try {

      val size = FileUtils.readBleInt(inputStream, 4)

      for (i <- 1 to size) {
        val term = FileUtils.readNullTerminatedString(inputStream)
        val totalFreq = FileUtils.readBleInt(inputStream, 4)
        val pointer = FileUtils.readBleLong(inputStream, 16)
        
        hmap.put(term, new TermEntry(term, totalFreq, pointer, 0, 0))
      }

    } finally {
      inputStream.close()
    }
    
    val inputStream2 = new DataInputStream(new BufferedInputStream(new FileInputStream(avocabularyFile)))
    try {
      val size = FileUtils.readBleInt(inputStream2, 4)

      for (i <- 1 to size) {
        val term = FileUtils.readNullTerminatedString(inputStream2)
		val aTotalFreq = FileUtils.readBleInt(inputStream2, 4)
		val apointer = FileUtils.readBleLong(inputStream2, 16)
        
//        if (aTotalFreq > 0) {
//          Console.printf("%s %d %d\n", term, aTotalFreq, apointer)
//        }
        
        val entry = hmap.get(term)
        entry.anchorFreq = aTotalFreq
        entry.anchorPointer = apointer
      }

    } finally {
      inputStream2.close()
    }

    new Vocabulary() {
      def findTerm(t: String): TermEntry = {
        val te = hmap.get(t)
        if (te != null) {
          te
          /*
          new TermEntry {
            val term = t
            val bodyFreq = te._1
            val anchorFreq = te._3
            def findOccurences = new OccurencesCursorImpl(te._1, indexFile, te._2)
            def findAnchorOccurences = new OccurencesCursorImpl(te._3, aindexFile, te._4)
          }
          */
        } else {
          new TermEntry(t, 0, 0, 0, 0)
        }
      }

      def iterator = {
        val iter = hmap.entrySet().iterator()
        new Iterator[TermEntry] {
          def hasNext = iter.hasNext()
          def next = {
            iter.next().getValue()
          }
        }
      }
      
      override def findOccurences(te: TermEntry) = new OccurencesCursorImpl(te.bodyFreq, indexFile, te.bodyPointer)
      override def findAnchorOccurences(te: TermEntry) = new OccurencesCursorImpl(te.anchorFreq, aindexFile, te.anchorPointer)
      
    }

  }
}

