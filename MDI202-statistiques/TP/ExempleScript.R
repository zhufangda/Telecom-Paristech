#' ---
#' title: "Exemple de Script facilement convertible en Notebook depuis RStudio"
#' output: html_document
#' author: Anne Sabourin
#' ---
#' 
#' 
#'Pour exécuter ce script, ouvrez le avec RStudio et pressez Ctr+Shift+Entrée. Pour l'exécuter et le convertir (compiler) 
#'en notebook html, pressez Ctrl+shift+K (il faut avoir installé le package `rmarkdown`).  
#'Pour obtenir un pdf plutôt qu'un html (déconseillé), remplacez
#'`output: html_document` par `output: pdf_document` dans l'en-tête ci-dessus. 
#'

print("exemple de calcul")
pommes = 3 ; 
poires = 8; 
fruits = pommes + poires ; 
fruits
## Ceci est une ligne de  commentaire
#' Ceci est une autre ligne de commentaire. Quelle est la différence ? 
#' pour le savoir: compilez le notebook depuis RStudio.  
#' 
#'**Figures**: elles apparaissent sous le code les ayant générées, automatiquement. 
#' Générons par exemple des données sous une loi normale standard et traçons l'histogramme:
X = rnorm(100)
hist(X)

## Ici je ferais des commentaires si je devais interpréter cette figure. 

#'  Dans un fichier script comme celui-ci, vous pouvez insérer des commentaires 
#'  (visibles dans le notebook mais pas exécutés) grâce aux symboles # ou #' . 
#'   Une fois le fichier exporté au format notebook (Menu -> File -> Compile Notebook ou Ctrl+Shift+K), 
#'     
#'    * Les lignes commençant par # ou ## apparaissent dans les "code chunks" 
#'  (les encadrés contenant du code et le résultat).   
#'  
#'    * Les lignes commençant par #' apparaissent normalement et le retour à la ligne 
#'  s'effectue automatiquement. Pour forcer le retour à la ligne: rajouter deux espaces 
#'  à la fin de la ligne comme ceci:  
#'  Nous sommes passés à la ligne suivante.  
#'  
#'  **Equations, symboles mathématiques**: Vous pouvez insérer des équations en langage LaTeX 
#'  dans les commentaires du script comme ceci: 
#'  $y = x+3$, ou  $y^3 = \cos\theta$ ; ou encore hors ligne comme ceci:  
#'  $$\mathbb{E}_\theta(X) = \int_{x\in\mathcal{X}} x \;  p_\theta(x) \mu(dx). $$
#'    
#'    
  








