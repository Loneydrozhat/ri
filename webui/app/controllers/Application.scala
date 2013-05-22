package controllers

import models.DocumentDb
import models.QueryEngine
import models.SearchParams
import models.Vocabulary
import play.api.Play
import play.api.Play.current
import play.api.data.Form
import play.api.data.Forms.mapping
import play.api.data.Forms.number
import play.api.data.Forms.text
import play.api.mvc.Action
import play.api.mvc.Controller

object Application extends Controller {

//  val vocabulary = Vocabulary.fromFile(Play.getFile("data/out.vocabulary.dat"), Play.getFile("data/out.index.dat"))
//  val documentDb = DocumentDb.fromFile(Play.getFile("data/out.docs.txt"))
  val vocabulary = Vocabulary.fromFile(Play.getFile("data/cri.vocabulary.dat"), Play.getFile("data/cri.index.dat"))
  val documentDb = DocumentDb.fromFile(Play.getFile("data/cri.docs.txt"))

  def index = Action {
    val params = SearchParams("", 50, 5000, "vector")
    Ok(views.html.search(params))
  }

  def search = Action { implicit request =>
    val searchForm = Form(mapping(
      "query" -> text,
      "r" -> number,
      "amax" -> number,
      "engine" -> text)(SearchParams.apply)(SearchParams.unapply))
    
    val params = searchForm.bindFromRequest.get
    
    val engine = params.engine match {
      case "vector" => QueryEngine.vector(vocabulary, documentDb)
      case "bm25" => QueryEngine.bm25(vocabulary, documentDb, 1.0, 0.75)
    }
    
    val result = engine.findDocuments(params.query, params.r, params.amax)

    Ok(views.html.searchResult(params, result))
  }

  def printVocabulary = Action {
    Ok(views.html.vocabulary(vocabulary))
  }
}