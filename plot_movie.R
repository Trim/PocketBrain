#!/usr/bin/Rscript
args<-commandArgs(TRUE)

pocketbrain=read.table(args[1], col.names=c("arousal", "valence", "emotion", "time"))
attach(pocketbrain)

arousalCalm <- read.table(args[2], col.names= c("calm"))
attach(arousalCalm)

arousalExited <- read.table(args[3], col.names= c("exited"))
attach(arousalExited)

x11()
boxplot(calm, arousal, exited, main="Arousal boxplots", names=c("Calm", "Movie arousal", "Exited"), outline=FALSE, horizontal=TRUE)

arousalPs = paste(args[1],".arousal_",tail(time, n=1), ".pdf", sep='')
dev.copy2pdf(file=arousalPs,width=6,height=6)

valencePositive <- read.table(args[4], col.names= c("positive"))
attach(valencePositive)

valenceNegative <- read.table(args[5], col.names= c("negative"))
attach(valenceNegative)

x11()
boxplot(positive, valence, negative, main="Valence boxplots", names=c("Positive","Movie valence", "Negative"), outline=FALSE, horizontal=TRUE)

valencePs = paste(args[1],".valence_",tail(time, n=1), ".pdf", sep='')
dev.copy2pdf(file=valencePs,width=6,height=6)

q(status=0)
