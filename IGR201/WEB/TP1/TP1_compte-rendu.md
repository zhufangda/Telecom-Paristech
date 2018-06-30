# TP1 Travaux Pratiques: Web et graphisme
## Partie 1 Découverte de SVG
### Exercice 1 - Tracé de base, groupement de primitives graphiques, rôle de la viewBox

* **ViewBox**: Le viewbox utilise 4 valeur pour definir un region rectangle. Seulement le partie de images
se situant dans ce region va afficher dans un canvas dont le size est difini par l'attributes width
et height de tag svg. Tous la partie d'image dehors de la region defini par viewbox va not afficher.
Ainsi, si le taille de viewbox est inférieur à le taille de canvas(width * height), le image dans le 
viewbox va agrandir.     

* **Transform**: Ce attribut signifie que tous les objects dans cette groupe est transformé avec un function indiqué par son valeur.
L'action de transformation peut etre rotation, translation, déformation, Agrandissement, réduction et etc

* **Superposition**: Si deux objects sont placés aux meme coordonnées, l'object postérieur va cacher celui antérieur.

## Exercice 2 – Association de comportement à une représentation graphique

* **onmouseclick**: déclencher un évemenment indiqué par son valeur lors que l'on click ce object.
* **onmouseover**: déclencher un évemenment indiqué par son valeur lors que l'on fait entrer le curseur sur l'élément.
* **onmousedown**: déclencher un évemenment indiqué par son valeur lors que l'on appuye (sans relâcher) sur le bouton gauche de la souris sur l'élément
* **onmouseup**: déclencher un évemenment indiqué par son valeur lors que l'on relâche le bouton gauche de la souris sur l'élément.

## Exercice 3 – Manipulation par script de la représentation graphique

Javascript nous permet de bien definir mon propre fonction avec laquelle on peut redéfinir l'action lors de l'occurence des évenement de "mouse"


