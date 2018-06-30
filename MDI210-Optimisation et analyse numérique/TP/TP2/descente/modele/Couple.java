package descente.modele;


/**
 * Modélise un couple de deux double(s) ; peut modéliser un point du plan réel ou un vecteur libre de ce même plan
 *
 */
public class Couple {
	public double x, y;
	
	public Couple() {
		super();
	}

	public Couple(double x, double y) {
		super();
		this.x = x;
		this.y = y;
	}
	
	/**
	 * Ajoute un couple au couple concerné (sans modifier celui-ci) et retourne le résultat
	 * @param v le couple à ajouter
	 * @return le résultat de l'addition
	 */
	public Couple ajoute(Couple v) {
		return new Couple(this.x + v.x, this.y + v.y);
	}
	
	/**<font color ="red">
	 * Multiplie le couple concerné par un double (sans modifier le couple concerné) et retourne le résultat
	 * @param t le multiplicateur
	 * @return le couple qui résulte de la multiplication
	 </font>*/
	public Couple mult(double t) {
		return new Couple(t * this.x, t * this.y);
	}
	
	/**
	 * Soustrait un couple du couple concerné (sans modifier celui-ci) et retourne le résultat
	 * @param v le couple a soustraire
	 * @return le résultat de la soustraction
	 */
	public Couple soustrait(Couple v) {
		return this.ajoute(v.mult(-1));
	}
	
	/**<font color ="red">
	 * Effectue le produit scalaire du couple concerné avec un autre couple
	 * @param v le couple avec lequel le produit scalaire est effectué
	 * @return le résultat du produit scalaire
	 * 
	 </font>*/
	public  double produitScalaire(Couple v) {
		return this.x * v.x + this.y * v.y;
	}
	
	/**<font color ="red">
	 * @return 
	 * - si le couple modélise un vecteur, calcule la norme du vecteur concerné
	 * <br>- si le couple modélise un point, calcule la distance de ce point à l'origine
	 </font>*/
	public double norme() {
		return Math.sqrt(this.x * this.x + this.y * this.y);
	}
	
	/**
	 * Lorsque le couple modélise un point, calcule la distance du point concerné à une autre point
	 * @param P le point pour lequel on veut calculer la distance
	 * @return la distance du point concerné au point P
	 */
	public double distance(Couple P) {
		return this.soustrait(P).norme();	
	}
	
	/**<font color ="red">
	 * Lorsque le point modélise un vecteur, indique si ce vecteur est perpendiculaire à un autre vecteur 
	 * @param v le vecteur comparé au vecteur concerné
	 * @return true si v est perpendiculaire au vecteur concerné et false sinon
	 </font>*/
	public boolean estPerpendiculaire(Couple v) {
		return Descente.estNul(this.produitScalaire(v));
	}
	
	/**<font color ="red">
	 * décompose un vecteur selon un repère formé de deux  vecteurs
	 * @param v le vecteur a décomposé
	 * @param v1 le premier vecteur du repère
	 * @param v2 le second vecteur du repère
	 * @return les deux compsantes de v sur v1 et v2 ou bien null si v1 et v2 sont parallèles
	 </font>*/
	public static Couple decompose(Couple v, Couple v1, Couple v2) {

		double determinant = v1.x * v2.y - v1.y * v2.x;
		
		if (Descente.estNul(determinant)) return null;
		
		double mu1, mu2;
		mu1 = (v.x * v2.y - v.y * v2.x) / determinant;
		mu2 = (v.y * v1.x - v.x * v1.y) / determinant;
			
		return new Couple(mu1, mu2);
	}
	
	@Override
	/**
	 * teste l'égalité d'un couple avec un autre
	 */
	public boolean equals(Object obj) {
		// TODO Auto-generated method stub
		if (obj == null) return false;
		Couple c = (Couple)obj;
		return Descente.estNul(this.x - c.x) && Descente.estNul(this.y - c.y);
	}
	
	/**
	 * tronque un double en imposant le nombre de chiffres après la virgule
	 * @param val le double à tronquer, qui ne sera pas modifié
	 * @param nb le nombre de chiffres après la virgule
	 * @return le double tronqué
	 */
	public static double tronquer(double val, int nb) {
		double puissance = 1;
		for (int i = 0; i < nb; i++) puissance *= 10;
		return Math.round(puissance * val) /puissance;
	}

	/** 
	 * tronque les deux composantes d'un couple (le couple concerné n'est pas modifié) et retourne le résultat
	 * @param nb le nombre de chiffres derrière la virgule
	 * @return le couple tronqué
	 */
	
	public Couple tronquer(int nb) {
		return new Couple(Couple.tronquer(this.x, nb), Couple.tronquer(this.y, nb));	
	}
	
	@Override
	public String toString() {
		return "(" + x + ", " + y + ")";
	}	
	
}
