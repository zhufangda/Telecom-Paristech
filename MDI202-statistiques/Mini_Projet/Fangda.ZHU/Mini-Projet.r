
filesSize<-read.table("xn.txt"， header = T)
mydata = filesSize$Byte
cat("Nombre de fichier："，nrow(filesSize))

h = hist(log(mydata), breaks = 15, probability= TRUE, xlab = "log(file size)",
      main="Histogramme de la taille des fichiers en échelle logarithmique")

x <- seq(min(log(mydata)), max(log(mydata)), length = 100)
y <- dnorm( x, mean = mean(log(mydata)), sd = sd(log(mydata)))
lines(x, y, col = "blue", lwd = 2)

400/399 * sd(log(mydata))^2 - sd(log(mydata))^2

h = hist(log(mydata), breaks = 50, probability= TRUE,
      main="Histogramme de la taille des fichiers en échelle logarithmique")

log_x <- seq(-2, max(log(mydata)), length = 1000)
y_normal <- dnorm( log_x, mean = mean(log(mydata)), sd = sd(log(mydata)))
lines(log_x, y_normal, col = "blue", lwd = 2)

densité_lnorm <- function(x, mean, sd){
    return (1.0/(x*sqrt(2*pi*sd^2)) * exp(-(log(x)-mean)^2/(2*sd^2)))
}


#yfit_2 <- densité_lnorm(exp(xfit_2), mean(log(x)), sd(log(x)))
y_dlnorme <- dlnorm(exp(log_x), meanlog = mean(log(mydata)), sdlog = sd(log(mydata)))


par(new = TRUE)
plot(log_x, y_dlnorme, type = "l", axes = FALSE, bty = "n", xlab = "", ylab = "", col='red')
axis(4, ylim=c(0,max(y_dlnorme)), col="red",col.axis="red")

## Add Legend
legend("topright",legend=c("Densité de ln(x) suivant lois normal","Densité de X suivant loi log-Nromal"),
  text.col=c("black","red"),pch=c(16,15),col=c("black","red"))

median(mydata)

exp(mean(log(mydata)))

qlnorm(p=0.95, mean=mean(log(mydata)),sd=sd(log(mydata)))

exp(sqrt(399/400)*sd(log(mydata))*qnorm(p=0.95, mean=0, sd=1)+mean(log(mydata)))

mean(log(mydata))

print(0.1+400/2)

mu = mean(log(mydata))
0.1 + 0.5 * sum((log(mydata) - mu)^2)

margin = seq(0,1,length=5000)  
apriori = dgamma(margin, shape=0.1, scale=10) 

plot(x=margin,y=apriori,col="red",type="l",ylim=c(0,40),main="Densité de loi",xlab="Valeur",ylab="Densité")

posteriori = dgamma(margin, shape=200.1, scale=1/1708.3) 
lines(x=margin,y=posteriori,col="orange",type="l")

abline(v=200.1/1708.3, col="green", lwd = "4")
abline(v=400/399/var(log(mydata)), col="blue", lwd = "2", lty = 2)


legend("topright",legend=c("Densité à priori","Densité à posteriori","Espérance à posteriori","Espérance MV"),col=c("red","orange","green","blue"),lty=c(1,1,1,2), cex=0.8)


L = 8 * qchisq(p=0.95, df = 399)
Accepte = sd(log(mydata))^2*400 <= L
Accepte

seuil = sqrt(sd(log(mydata))^2 * 400 / qchisq(p=0.95,df = 399))
seuil
