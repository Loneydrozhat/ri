package main

object ChartHelper {

  def rPage(fname: String, plots: String): String = {
    val sb = new StringBuilder
    sb.append("pdf(file=\"%s.pdf\", height=12, width=8)\n".format(fname))
    sb.append("par(mfrow=c(3,2))\n")
    sb.append(plots);
    sb.append("dev.off()\n")
    sb.toString
  }
  
  def rPlot(query: String, names: List[String], values: Array[Array[Tuple2[Double, Double]]]): String = {

    val sb = new StringBuilder

    sb.append(
      """
plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("%s")
""".format(query))

    for (i <- 0 until values.length) {

      val sx: List[Double] = values(i).toList.map(_._1)
      val sy: List[Double] = values(i).toList.map(_._2)

      sb.append(
        """
x <- c(%s)
y <- c(%s)
lines(x, y, type="o", lty=%d, pch=%d)
""".format(sx.mkString(", "), sy.mkString(", "), 1 + i, 20 + i))

    }

    val snames = names.map("\"" + _ + "\"").mkString(", ")

    sb.append(
      """
legend("topright", c(%s), cex=0.8, lty=1:%d, pch=20:%d, lwd=1, bty="n")
""".format(snames, names.size, 20 + names.size))

    sb.toString

  }

}