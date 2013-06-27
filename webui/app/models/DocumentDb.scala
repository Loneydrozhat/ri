package models

import java.io.BufferedReader
import java.io.File
import java.io.FileReader
import java.lang.Double
import java.util.ArrayList

trait DocumentDb {

  def get(id: Int): Document

  def count: Int

  def avgLength: Double
}

class Document(val url: String, val length: Int, val norm: Double, val indegree: Int, val pr: Double) extends Comparable[Document] {
  override def compareTo(arg0: Document) = {
	Double.compare(this.pr, arg0.pr)
  }
}

object DocumentDb {
  def fromFile(documentsFile: File, prFile: File): DocumentDb = {

    val array = new ArrayList[Document]
    var lengthSum = 0.0;

    val docsFileReader = new BufferedReader(new FileReader(documentsFile))
    val prFileReader = new BufferedReader(new FileReader(prFile))
    try {
      var docLine = docsFileReader.readLine()
      var prLine = prFileReader.readLine()
      while (docLine != null && !docLine.isEmpty()) {
        val docParts = docLine.split(' ')
        val url = docParts(0)
        val length = Integer.parseInt(docParts(1))
        val norm = Double.parseDouble(docParts(2))
        
        val prParts = prLine.split(' ')
        val indegree = Integer.parseInt(prParts(0))
        val pr = Double.parseDouble(prParts(1))
        
        array.add(new Document(url, length, norm, indegree, pr))
        lengthSum += length
        docLine = docsFileReader.readLine()
        prLine = prFileReader.readLine()
      }

    } finally {
      docsFileReader.close()
      prFileReader.close()
    }

    val avgl = lengthSum / array.size()

    new DocumentDb {
      def get(id: Int) = array.get(id - 1)
      def count = array.size
      def avgLength = avgl
    }

  }
}