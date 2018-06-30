package probleme;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Descente;
import descente.modele.Domaine;

/**
 * Modélise un problème de minimisation d'une fonction sur un domaine limité par des contraintes affines
 */
public abstract class Pb {
	public Domaine domaine;

	/**
	 * La fonction à optimiser
	 * @param P		le couple  (x, y) 
	 * @return		la valeur de la fonction
	 */
	public abstract double f(Couple P);

	/**
	 * Le gradient de la fonction à minimiser
	 * @param P le point où est calculé le gradient
	 * @return une variable de type Couple contenant le gradient de f au point P
	 */
	public abstract Couple gradientf(Couple P);

	/**
	 * Si t-> P0 + td est l'équation paramétrique d'une demi-droite, calcule la dérivée de la fonction t -> f(P0 + td)
	 * @param P0 l'origine de la demi-droite
	 * @param d la direction de la demi-droite
	 * @param t la valeur du paramètre pour lequel on calcule la dérivée
	 * @return le résultat du calcul
	 */
	public double gPrime(Couple P0,  Couple d, double t) {
		Couple P = P0.ajoute(d.mult(t));
		return d.produitScalaire(gradientf(P));
	}
	
	public String toString() {
		String chaine ;
		if (this.domaine.getContraintes().size() != 0 ) {
			chaine = "avec :\n";
			for (Contrainte c : this.domaine.getContraintes()) 
				chaine += c + "\n";
		}
		else chaine = "Pas de contraintes";
		return chaine;
	}
	
	/**
	 * @return le domaine du problème
	 */
	public Domaine getDomaine() {
		return domaine;
	}

}