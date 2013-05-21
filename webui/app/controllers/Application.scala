package controllers

import models.QueryEngine
import models.VectorialModel
import play.api.Play
import play.api.Play.current
import play.api.data.Form
import play.api.data.Forms.text
import play.api.mvc.Action
import play.api.mvc.Controller

object Application extends Controller {

  private var queryEngine_ : QueryEngine = null
  
  def queryEngine: QueryEngine = {
    if (queryEngine_ == null) {
      queryEngine_ = QueryEngine.init(
      Play.getFile("data/out.vocabulary.dat"),
      Play.getFile("data/out.index.dat"),
      Play.getFile("data/out.docs.txt"))
    }
    queryEngine_
  }

  def index = Action {

    Play.getFile("out.docs.txt");

    Ok(views.html.search("Your new application is ready."))
  }

  def search = Action { implicit request =>
    val searchForm = Form(
      "query" -> text)
    val query = searchForm.bindFromRequest.get

    val k = 10
    val rankingModel = new VectorialModel()
    val result = queryEngine.findDocuments(rankingModel, query, k)

    Ok(views.html.searchResult(query, result))
  }

  def vocabulary = Action {
    Ok(views.html.vocabulary(queryEngine.vocabulary))
  }
}