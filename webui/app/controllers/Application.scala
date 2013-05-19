package controllers

import models.QueryResult
import play.api.Play
import play.api.Play.current
import play.api.data.Form
import play.api.data.Forms.text
import play.api.mvc.Action
import play.api.mvc.Controller

object Application extends Controller {
  
  def index = Action {
    
    Play.getFile("out.docs.txt");
    
    Ok(views.html.search("Your new application is ready."))
  }
  
  def search = Action {	implicit request =>
    val searchForm = Form(
      "query" -> text
    )
    
    val query = searchForm.bindFromRequest.get;
    
    Ok(views.html.searchResult(query, List(QueryResult("doodle", 1.0), QueryResult("bwahoo", 0.5))))
  }

}