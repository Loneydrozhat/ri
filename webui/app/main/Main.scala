package main

import java.io.File

import scala.collection.mutable.ListBuffer
import scala.io.Source

import models.DocumentDb
import models.QueryEngine
import models.Vocabulary

object Main extends App {

  val inputDir = new File(args(0))
  val queryFiles = inputDir.listFiles.filter(_.isFile())

  val vocabulary = Vocabulary.fromFile(new File("data/cri.vocabulary.dat"), new File("data/cri.index.dat"))
  val documentDb = DocumentDb.fromFile(new File("data/cri.docs.txt"))
  //  val vocabulary = Vocabulary.fromFile(new File("data/out.vocabulary.dat"), new File("data/out.index.dat"))
  //  val documentDb = DocumentDb.fromFile(new File("data/out.docs.txt"))

  val qengine = QueryEngine.bm25(vocabulary, documentDb)
  Console.println("initialized")

  val r = 1000000
  val accumulators = 1000000

  for (queryFile <- queryFiles) {
    val query = queryFile.getName()
    val relevants = relevantSet(queryFile)
    val result = qengine.findDocuments(query, r, accumulators)
    val actual = result.map(_.url)

    Console.println(query)
    Console.println(actual.size + " documentos recuperados")
    val series = precisionAtRecall(actual, relevants)
    for (v <- series) {
      Console.printf("%.2f; %.2f\n", v._1, v._2)
    }

  }
  // */
  
  /*
  val series = precisionAtRecall(List("nabo", "banana", "laranja"), Set("banana", "laranja"))
  for (v <- series) {
    Console.printf("%.2f; %.2f\n", v._1, v._2)
  }
  //*/

  def precisionAtRecall(actual: Seq[String], relevants: Set[String]): List[Tuple2[Double, Double]] = {
    var values = new ListBuffer[Tuple2[Double, Double]]

    var relevantsFound = 0
    var totalFound = 0
    var totalRelevants = relevants.size
    for (doc <- actual) {
      totalFound += 1
      if (relevants.contains(doc)) {
        relevantsFound += 1
        val recall = 100.0 * (relevantsFound.toDouble / totalRelevants)
        val precision = 100.0 * (relevantsFound.toDouble / totalFound)
        values += Tuple2(recall, precision)
        //Console.printf("%.2f; %.2f\n", recall, precision)
      }
    }
    if (relevantsFound < totalRelevants) {
      values += Tuple2(100.0, 0.0)
    }
    values.toList
  }

  def relevantSet(file: File): Set[String] = {
    Source.fromFile(file).getLines().filterNot(_.isEmpty()).toSet
  }

}