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

case class Document(url: String, length: Int, norm: Double)

object DocumentDb {
  def fromFile(documentsFile: File): DocumentDb = {

    val array = new ArrayList[Document]
    var lengthSum = 0.0;

    val fileReader = new BufferedReader(new FileReader(documentsFile))
    try {
      var line = fileReader.readLine()
      while (line != null && !line.isEmpty()) {
        val parts = line.split(' ')
        val length = Integer.parseInt(parts(1))
        val norm = Double.parseDouble(parts(2))
        array.add(Document(parts(0), length, norm))
        lengthSum += length
        line = fileReader.readLine()
      }

    } finally {
      fileReader.close()
    }

    val avgl = lengthSum / array.size()

    new DocumentDb {
      def get(id: Int) = array.get(id - 1)
      def count = array.size
      def avgLength = avgl
    }

  }
}