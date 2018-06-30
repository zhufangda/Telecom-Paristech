# Compte-Rendu de Partie 2 SVG et HTML

## EXERCICE 2
Quand le taille de l'image est très grand, il va sauter à ligne et prendre un ligne entière pour l'afficher. Le taille de web page va aussi étendre pour contenir cet image.
Quand le taille de l'image est très petit, l'image va réduir comme un tache.

## EXERCICE 4
Le texte et les autres éléments en ligne (inline) entoureront l'élément avec l'attribute "float:left" situé à left.

# Travaux Pratiques - Web et graphisme

## Exercice 1

Canvas utilise le code javascripe to dessiner les formes geometriques dynamiquement. Le svg est un fichier xml qui enregistre tous les information sur le dessin vectoriel et desiner par navigateur.

Pour agrandir le svg, on ajouter un attribute dans svg tag comme <code> transforme = "scale" </code>.
Pour agrandir le cancas, on utilse la fonction <code> ctx.scale</code> dans le partie de javascripe.

## Exercice 2
Sachant que le <code> <canvas> </code> dans son propre region est seulement un bitmap. Alors on ne pouvais pas attributer un onClick comportement à un partie de son contenue. Mais on peut ajouter un region interative dedans par la fonction <code> CanvasRenderingContext2D.addHitRegion() <code> et 
	<code>canvas.addEventListener('mousemove', function(event)</code>.


## Exercice 3
Quand on click le region de canvas, on peut obtenir le postion de mouse par un pop-dialog. 
Dans exercice, on ajoute un listener à ce canvas.Ce listener va survailler tous les évenement de click sur canvas. 

## Exercice 4
On peut ajouter un listener afin de survailler l'evement "mouseover" et l'evement "mouseout" et ajouter des operations correspondant.(See my code)

## Exercice 5
1. Avantage de SVG
- Un Graphiques vectoriels
- Les formes peuvent être modidié par le code
- L'interaction modèle de l'évenement est abstrite(chmin, rect)
- un format d'image, on peut utilse partout.
- le taille de fichier est petit


2. inconvénient de SVG
- Ne performe pas bien pour grande nombre d'objet

3. Avantage de canvas 
- performe bien pour grande nombre d'objet

4. Inconvénient de Canvas
- base sur bitmap
- L’interaction modèle/utilisateur de l’événement est granulaire (x,y)

