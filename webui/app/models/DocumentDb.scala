package models

import java.io.BufferedReader
import java.io.File
import java.io.FileReader
import java.util.ArrayList

trait DocumentDb {
  def get(id: Int): Document
}

case class Document(url: String, length: Int, norm: Double)

object DocumentDb {
  def fromFile(documentsFile: File): DocumentDb = {

	val array = new ArrayList[Document]
	
    val fileReader = new BufferedReader(new FileReader(documentsFile))
    try {
      var line = fileReader.readLine()
      while (line != null && !line.isEmpty()) {
        val parts = line.split(' ')
        val length = Integer.parseInt(parts(1))
        val norm = Math.sqrt(Integer.parseInt(parts(2)))
        array.add(Document(parts(0), length, norm))
        line = fileReader.readLine()
      }
      
    } finally {
      fileReader.close()
    }

    new DocumentDb {
      def get(id: Int) = array.get(id - 1)
    }

  }
}