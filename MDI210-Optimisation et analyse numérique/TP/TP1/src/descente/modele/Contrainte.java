package descente.modele;

/**
 * Modélise une contrainte affine qui s'écrit : coeffx * x + coeffy * y + constante <= 0 .
 * La droite d'équation : coeffx * x + coeffy * y + constante = 0 est la droite "frontière".
 */
public class Contrainte {
	private final double coeffx;
	private final double coeffy;
	private final double constante;
	private Couple gradient;     // le gradient de la fonction : (x, y) -> coeffx * x + coeffy * y + constante
	private Couple bordUnitaire; // un vecteur unitaire parallèle à la droite frontière

	/**
	 * @param coeffx le coefficient de x
	 * @param coeffy le coefficient de y
	 * @param constante la constante
	 */
	public Contrainte(double coeffx, double coeffy, double constante) {
		super();
		this.coeffx = coeffx;
		this.coeffy = coeffy;
		this.constante = constante;
		this.gradient = new Couple(coeffx, coeffy);
		this.bordUnitaire = new Couple(-coeffy, coeffx);
		this.bordUnitaire = this.bordUnitaire.mult(1/this.bordUnitaire.norme());
	}
	
	/**
	 * Calcule la valeur de coeffx * x + coeffy * y + constante pour un couple de réel donné
	 * @param P le point en lequel on calcule la valeur de la fonction affine
	 * @return le résultat du calcul
	 */
	public double valeur(Couple P) {
		return this.coeffx * P.x + this.coeffy * P.y + this.constante;
	}

	/**
	 * Recherche si le point P vérifie ou non la contrainte
	 * @param P 	le point considéré
	 * @return 		true si le point vérifie la contrainte, false sinon
	 */
	public boolean estVerifie(Couple P) {
		double val = this.valeur(P);
		return  val <= 0 || Descente.estNul(val);	
	}

	/**
	 * Recherche si le point P sature ou non la contrainte, c'est-à-dire appartient à la droite frontière
	 * @param P 	le point considere
	 * @return 		true si le point appartient à la droite frontière, false sinon
	 */
	public boolean estSature(Couple P) {
		return Descente.estNul(valeur(P));	
	}

	/**
	 * On cherche le point d'intersection de la droite frontière avec la demi-droite paramétrée pour t > 0 par : 
	 *  t -> P0 + td 
	 * @param P0 l'origine de la demi-droite avec laquelle on cherche l'intersection
	 * @param d le vecteur directeur de la demi-droite
	 * @return s'il y a intersection, la valeur du paramètre t correspondant à l'intersection, 
	 *         sinon -1 
	 */
	public double intersection(Couple P0, Couple d) {
		double t;
		double produit = d.produitScalaire(gradient);

		if (Descente.estNul(produit)) return -1;
		t = (-constante - P0.produitScalaire(gradient)) / produit;
		return t;	
	}

	/**
	 * Calcule l'intersection de la droite frontière de la contrainte concernée 
	 * avec la droite frontière d'une autre contraintes
	 * @param c la contrainte pour laquelle on calcule l'intersection
	 * @return le point d'intersection des deux droites, et null si les droites sont parallèles
	 */
	public Couple intersection(Contrainte c) {
		double determinant = this.coeffx * c.coeffy - this.coeffy * c.coeffx;

		if (Descente.estNul(determinant)) return null;

		Couple I = new Couple();
		I.x = (c.constante * this.coeffy - this.constante * c.coeffy) / determinant;
		I.y = (this.constante * c.coeffx - c.constante * this.coeffx) / determinant;

		return I;	
	}

	/**
	 * Etudie le cas de contraintes parallèles
	 * @param c	la contrainte comparée a this
	 * @return 	0 si la contrainte c n'est pas parallele a this
	 * 			<br> -1 si la contrainte c est parallele a this, "dans le meme sens" et que c rend this inutile
	 * 			<br> 1 si la contrainte c est parallele a this "dans le meme sens" et que this rend c inutile
	 * 			<br> -2 si la contrainte c est parallele a this en sens contraire et incompatible 
	 * 			<br> 2 si la contrainte c est paralleles a this en sens contraire et compatible 
	 */
	public int estParallele(Contrainte c) {
		Couple g = this.getGradient();
		Couple g1 = c.getGradient();

		if (!g.estPerpendiculaire(c.getBordUnitaire())) return 0;
		double produit = g.produitScalaire(g1);
		if (produit > 0) 
			if (this.estVerifie(c.unPoint())) return -1;
			else return 1;
		else 
			if (this.estVerifie(c.unPoint())) return 2;
			else return -2;
	}

	/**
	 * Cherche un point de la droite frontière
	 * @return un point de la droite frontière
	 */
	public Couple unPoint() {
		if (this.coeffy != 0) return new Couple(0, -this.constante / this.coeffy);
		else return new Couple(-this.constante / this.coeffx, 0);
	}

	@Override
	public String toString() {
		String chaine;
		boolean signe = true;
		if (this.coeffx == (int)this.coeffx)
			if (this.coeffx == 0) {
				chaine = "      ";
				signe = false;
			}
			else if (this.coeffx == 1) chaine = "   x  ";
			else if (this.coeffx == -1) chaine = "  -x  ";
			else chaine =  (int)this.coeffx + " x ";
		else 
			chaine = this.coeffx + " x  ";

		if (this.coeffy == (int)this.coeffy) 
			if (this.coeffy == 0) chaine += "       <=  ";
			else if (this.coeffy == 1) 
				if (signe) chaine += " +   y  <=  ";
				else chaine += "      y >=  ";
			else if (this.coeffy == -1) 
				if (signe) chaine += " -   y  <=  ";
				else chaine += "   - y <=  ";
			else if (this.coeffy < 0) 
				if (signe) chaine += " - " + (-(int)this.coeffy) +  " y  <=  ";
				else chaine += "     " + ((int)this.coeffy) +  " y  <=  ";
			else chaine += " + " + (int)this.coeffy +  " y  <=  ";
		else 	
			if (this.coeffy > 0) 
				if (signe) chaine += "+ " + this.coeffy + " y <= ";
				else chaine += " " + this.coeffy + " y <= ";
			else if (this.coeffy > 0) chaine += "         <= ";
			else chaine += "- " + (-this.coeffy) + " y <= ";	

		if (this.constante == (int)this.constante) chaine += -(int)this.constante;
		else chaine += -this.constante;
		return chaine;
	}

	/**<font color="red">
	 * @return un vecteur unitaire de la droite frontière
	 </font>*/
	public Couple getBordUnitaire() {
		return this.bordUnitaire;
	}

	/**<font color="red">
	 * @return le gradient de la fonction : (x, y) -> coeffx * x + coeffy * y + constante
	 </font>*/
	public Couple getGradient() {
		return this.gradient;
	}

	/**
	 * @return le coefficient de x dans l'équation : (x, y) -> coeffx * x + coeffy * y + constante 
	 * donnant la droite frontière
	 */
	public double getCoeffx() {
		return this.coeffx;
	}
	
	/**
	 * @return le coefficient de y dans l'équation : (x, y) -> coeffx * x + coeffy * y + constante 
	 * donnant la droite frontière
	 */
	public double getCoeffy() {
		return this.coeffy;
	}

	/**
	 * @return la constante dans l'équation : (x, y) -> coeffx * x + coeffy * y + constante 
	 * donnant la droite frontière
	 */
	public double getConstante() {
		return this.constante;
	}
}
