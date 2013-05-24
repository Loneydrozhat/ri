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
  //val vocabulary = Vocabulary.fromFile(new File("data/out.vocabulary.dat"), new File("data/out.index.dat"))
  //val documentDb = DocumentDb.fromFile(new File("data/out.docs.txt"))
  Console.println("#initialized")

  val vector = QueryEngine.vector(vocabulary, documentDb)
  val bm25_075 = QueryEngine.bm25(vocabulary, documentDb, 1.0, 0.8)
  //val vbm25_05 = QueryEngine.vectorBm25(vocabulary, documentDb, 0.5, 1.0, 0.8)
  //val vbm25_up = QueryEngine.vectorBm25(vocabulary, documentDb, 0.5, 1.0, 0.8)
  //val vbm25_08 = QueryEngine.vectorBm25(vocabulary, documentDb, 0.8, 2.0, 0.8)
  val vbm1 = QueryEngine.vectorBm25(vocabulary, documentDb, 0.5, 1.0, 0.8)
  val vbm2 = QueryEngine.vectorBm25(vocabulary, documentDb, 0.9, 1.0, 0.8)

  val engines = List(vector, bm25_075, vbm1, vbm2)

  val r = 1000000
  val accumulators = 1000000

  var count = 1;
  var plotsOnPage = new StringBuffer
  
  for (queryFile <- queryFiles) {
    val query = queryFile.getName()
    val relevants = relevantSet(queryFile)
    //Console.println(query)

    val values = new ListBuffer[Array[Tuple2[Double, Double]]]
    val names = ListBuffer[String]()
    
    for (engine <- engines) {
      
      val result = engine.findDocuments(query, r, accumulators)
      val actual = result.map(_.url)
      //Console.println(actual.size + " documentos recuperados")

      val series = precisionAtRecall(actual, relevants)

      names += engine.name
      values += series
      //for (v <- series) {
        //Console.printf("%.2f; %.2f\n", v._1, v._2)
      //}
    }

    plotsOnPage.append(ChartHelper.rPlot(query, names.toList, values.toArray))
    if (count % 6 == 0) {
      val rpage = ChartHelper.rPage(("page" + (count / 6)), plotsOnPage.toString)
      Console.println(rpage)
      plotsOnPage = new StringBuffer
    }
    count += 1
  }

  if (plotsOnPage.length() > 0) {
    val rpage = ChartHelper.rPage(("page" + ((count / 6) + 1)), plotsOnPage.toString)
    Console.println(rpage)
  }

  def precisionAtRecall(actual: Seq[String], relevants: Set[String]): Array[Tuple2[Double, Double]] = {
    var values = new ListBuffer[Tuple2[Double, Double]]

    val relevantsSeen = collection.mutable.Set[String]()

    var relevantsFound = 0
    var totalFound = 0
    var totalRelevants = relevants.size
    for (doc <- actual) {
      // Ignora duplicatas
      if (!relevantsSeen.contains(doc)) {
        totalFound += 1
        if (relevants.contains(doc)) {
          relevantsSeen += doc
          relevantsFound += 1
          val recall = 100.0 * (relevantsFound.toDouble / totalRelevants)
          val precision = 100.0 * (relevantsFound.toDouble / totalFound)
          values += Tuple2(recall, precision)
          //Console.printf("%.2f; %.2f\n", recall, precision)
        }

      }
    }
    if (relevantsFound < totalRelevants) {
      values += Tuple2(100.0, 0.0)
    }
    values.insert(0, Tuple2(0.0, values(0)._2))  
    
    values.toArray
  }

  def relevantSet(file: File): Set[String] = {
    Source.fromFile(file).getLines().filterNot(_.isEmpty()).toSet
  }

}