#initialized
pdf(file="page1", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("baixaki")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("jogos online")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("receita federal")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("poquer")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("ig")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("claro")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

pdf(file="page2", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("globo")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("concursos")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("terra")

x <- c(0.0, 4.545454545454546, 100.0)
y <- c(50.0, 50.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 4.545454545454546, 100.0)
y <- c(50.0, 50.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 4.545454545454546, 100.0)
y <- c(50.0, 50.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("record")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("google")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("vivo")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

pdf(file="page3", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("orkut")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("rio de janeiro")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("uol")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("mario")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("frases de amor")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("jogos de meninas")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

pdf(file="page4", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("funk")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("caixa economica federal")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("mercado livre")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("youtube")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("naruto")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("oi")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

pdf(file="page5", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("casa e video")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("esporte")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("gmail")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("detran")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("previsao do tempo")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("yahoo")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

pdf(file="page6", height=12, width=8)
par(mfrow=c(3,2))

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("ana maria braga")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("hotmail")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("msn")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")

plot(c(1, 100), c(1, 100), type="n", xlab="Revocação", ylab="Precisão")
title("panico")

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=1, pch=20)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=2, pch=21)

x <- c(0.0, 100.0)
y <- c(0.0, 0.0)
lines(x, y, type="o", lty=3, pch=22)

legend("topright", c("vector", "bm25[k1=1.0;b=0.8]", "vbm[c=0.5;k1=1.0;b=0.8]"), cex=0.8, lty=1:3, pch=20:23, lwd=1, bty="n")
dev.off()

