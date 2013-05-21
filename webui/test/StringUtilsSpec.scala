package test

import org.specs2.mutable.Specification

import utils.StringUtils

trait StringUtilsSpec extends Specification {

  "StringUtils.normalize" should {

    "remove accents" in {
      val output = StringUtils.normalize("áãéó")
      output must equalTo("aaeo")
    }

  }
}