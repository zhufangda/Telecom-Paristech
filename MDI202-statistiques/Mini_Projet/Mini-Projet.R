
filesSize<-read.table("E:/SD/MDI202-statistiques/Mini_Projet/xn.txt", header = T)
data = filesSize$Byte

h = hist(log(mydata), breaks = 100, probability= TRUE,
         main="Histogramme de la taille des fichiers en échelle logarithmique")

log_x = seq(min(log(data)), max(log(data)), length.out = 400)
y = dnorm(log_x, mean=mean(log(data)), sd=sd(log(data)))

par(new = TRUE)
plot(log_x,y)


x = seq(min(data), max(data), length.out = 4000000)
yl = dlnorm( x, mean=mean(data), sdlog=sd(data))


plot(x,yl)



library(ggplot2)
