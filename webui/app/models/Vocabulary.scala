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
}

trait TermEntry {
  val term: String
  val totalFreq: Int
  def findOccurences: OccurencesCursor
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
  def fromFile(vocabularyFile: File, indexFile: File): Vocabulary = {

    val inputStream = new DataInputStream(new BufferedInputStream(new FileInputStream(vocabularyFile)))

    val hmap = new HashMap[String, Tuple2[Int, Long]]

    try {

      val size = FileUtils.readBleInt(inputStream, 4)

      for (i <- 1 to size) {
        val term = FileUtils.readNullTerminatedString(inputStream)
        val totalFreq = FileUtils.readBleInt(inputStream, 4)
        val pointer = FileUtils.readBleLong(inputStream, 16)
        
//        Console.printf("%s %d %d\n", term, totalFreq, pointer)
        
        hmap.put(term, (totalFreq, pointer))
      }

    } finally {
      inputStream.close()
    }

    new Vocabulary() {
      def findTerm(t: String): TermEntry = {
        val te = hmap.get(t)
        if (te != null) {
          new TermEntry {
            val term = t
            val totalFreq = te._1
            def findOccurences = new OccurencesCursorImpl(te._1, indexFile, te._2)
          }
        } else {
          new TermEntry {
            val term = t
            val totalFreq = 0
            def findOccurences = NilCursor
          }
        }
      }

      def iterator = {
        val iter = hmap.entrySet().iterator()
        new Iterator[TermEntry] {
          def hasNext = iter.hasNext()
          def next = {
            val entry = iter.next()
            new TermEntry {
              val term = entry.getKey()
              val totalFreq = entry.getValue()._1
              def findOccurences = NilCursor
            }
          }
        }
      }
    }

  }
}

