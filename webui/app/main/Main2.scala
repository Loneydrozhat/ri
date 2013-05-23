package main

import java.io.File

import scala.collection.mutable.ListBuffer
import scala.io.Source

import models.DocumentDb
import models.QueryEngine
import models.Vocabulary

object Main2 extends App {

  val vocabulary = Vocabulary.fromFile(new File("data/out.vocabulary.dat"), new File("data/out.index.dat"))
  
  val tt = vocabulary.findTerm("de")
  
  val cursor = tt.findOccurences
  while (cursor.fetchNext) {
    val x = cursor.documentId
    Console.printf("%d %d\n", cursor.documentId, cursor.freq)
  }
  cursor.close
}