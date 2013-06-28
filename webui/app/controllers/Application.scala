package controllers

import scala.collection.mutable.MutableList
import scala.io.Source
import scala.math.BigDecimal.double2bigDecimal

import models.Document
import models.DocumentDb
import models.QueryEngine
import models.SearchParams
import models.Vocabulary
import play.api.Play
import play.api.Play.current
import play.api.data.Form
import play.api.data.Forms.bigDecimal
import play.api.data.Forms.mapping
import play.api.data.Forms.number
import play.api.data.Forms.text
import play.api.mvc.Action
import play.api.mvc.Controller
import utils.HeapList

object Application extends Controller {

//  val vocabulary = Vocabulary.fromFile(Play.getFile("data/out.vocabulary.dat"), Play.getFile("data/out.index.dat"), Play.getFile("data/out.avocabulary.dat"), Play.getFile("data/out.aindex.dat"))
//  val documentDb = DocumentDb.fromFile(Play.getFile("data/out.docs.txt"), Play.getFile("data/out.pr.txt"))
  val vocabulary = Vocabulary.fromFile(Play.getFile("data/cri.vocabulary.dat"), Play.getFile("data/cri.index.dat"), Play.getFile("data/cri.avocabulary.dat"), Play.getFile("data/cri.aindex.dat"))
  val documentDb = DocumentDb.fromFile(Play.getFile("data/cri.docs.txt"), Play.getFile("data/cri.pr.txt"))

  def index = Action {
    val params = SearchParams("", 50, 5000, "bm25-anchor-pr", 1.0, 0.75)
    Ok(views.html.search(params))
  }

  def search = Action { implicit request =>
    val searchForm = Form(mapping(
      "query" -> text,
      "r" -> number,
      "amax" -> number,
      "engine" -> text,
      "k1" -> bigDecimal,
      "b" -> bigDecimal)(SearchParams.apply)(SearchParams.unapply))

    val params = searchForm.bindFromRequest.get

    val qfile = Play.getFile("queries/" + params.query)
    val relevants: Set[String] =
      if (qfile.exists() && qfile.isFile())
        Source.fromFile(qfile).getLines().filterNot(_.isEmpty()).toSet
      else
        Set()

    val engine = params.engine match {
      case "vector" => QueryEngine.vector(vocabulary, documentDb)
      case "bm25" => QueryEngine.bm25(vocabulary, documentDb, params.k1.doubleValue, params.b.doubleValue)
      case "bm25-anchorOnly" => QueryEngine.bm25AOnly(vocabulary, documentDb, params.k1.doubleValue, params.b.doubleValue)
      case "bm25-anchor" => QueryEngine.bm25a(vocabulary, documentDb, params.k1.doubleValue, params.b.doubleValue, 2.0)
      case "bm25-anchor-pr" => QueryEngine.bm25pra(vocabulary, documentDb, params.k1.doubleValue, params.b.doubleValue, 2.0)
      case "combined" => QueryEngine.vectorBm25(vocabulary, documentDb, 0.5, params.k1.doubleValue, params.b.doubleValue)
    }

    val result = engine.findDocuments(params.query, params.r, params.amax)

    Ok(views.html.searchResult(params, result, relevants))
  }

  def printVocabulary = Action {
    Ok(views.html.vocabulary(vocabulary))
  }
  
  def printPagerank = Action {
    
    val heap = new HeapList[Document](Math.min(documentDb.count, 250))
    for (i <- 1 to documentDb.count) {
      val doc = documentDb.get(i);
      heap.pushAndReplaceMin(doc);
    }
    
    val result: MutableList[Document] = MutableList()
    while (heap.size() > 0) {
      heap.popMin() +=: result
    }
    
    Ok(views.html.pagerank(result.toList))
  }
}