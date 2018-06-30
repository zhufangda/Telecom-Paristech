package descente.modele;

import java.util.ArrayList;

/**
 * Modélise un domaine défini par un ensemble de contraintes affines
 * On suppose qu'il n'existe pas trois droites déterminant les contraintes qui se coupent en un même point.
 */
public class Domaine {
	private ArrayList<Contrainte> contraintes; 
	private double minorant_x;
	private double majorant_x;
	private double minorant_y;
	private double majorant_y;
	private boolean vide;
	private ArrayList<Bord> bords = new ArrayList<Bord>();
	private ArrayList<Couple> coins = new ArrayList<Couple>();

	/**
	 * Constructeur
	 * @param contraintes l'ensemble des contraintes du domaine
	 * @param calculer indique s'il faut ou non calculer les bords, les coins du domaine, les bornes du domaine
	 */
	public Domaine(ArrayList<Contrainte> contraintes, boolean calculer) {
		super();
		this.contraintes = contraintes;
		if (calculer) {
			this.calculerBordsDomaine();
			this.epurer();
			this.calculerCoinsDomaine();
			this.calculerBornes();
		}
	}

	/**
	 * Recherche si un point est ou non dans le domaine
	 * @param P le point dont on recherche s'il est dans le domaine
	 * @return true si P est dans le domaine, false sinon
	 */
	public boolean estRealisable(Couple P) {
		boolean dedans = true;

		for (Contrainte c : this.contraintes) {
			if (!c.estVerifie(P)) {
				dedans = false;
				break;
			}
		}
		return dedans;
	}

	/**<font color ="red">
	 * Cherche si un point donné est sur un bord du domaine
	 * @return si P est sur un bord, une contrainte sur lequel se trouve P
	 *         sinon null.   
	 </font>*/
	public Contrainte estSurBord (Couple P) {
		for (Contrainte c : this.contraintes) 
			if (c.estSature(P)) return c;
		return null;
	}

	/**<font color ="red">
	 * Etant donné deux contraintes, cherche si l'intersection de leurs droites frontières est un coin du domaine
	 * @param c1 la première contrainte
	 * @param c2 la seconde contrainte
	 * @return si les deux droites s'intersectent en un coin du domaine, le point d'intersection
	 *         sinon null
	 </font>*/
	public Couple estCoin(Contrainte c1, Contrainte c2) {
		Couple P;
		boolean dedans = true;

		if (c1 == c2) return null;
		P = c1.intersection(c2);
		if (P == null) return null;
		for (Contrainte c : this.contraintes) {
			if ((c1 != c) && (c2 != c) && !c.estVerifie(P)) dedans = false;
		}
		if (dedans) return P;
		return null;
	}

	/**
	 * Recherche si un point donné est en un coin du domaine
	 * @param P le point considéré
	 * @return si P est en un coin du domaine, un tableau contenant les deux contraintes formant le coin,
	 *         sinon null
	 */
	public Contrainte[] estCoin(Couple P) {
		Contrainte[] lesDeux = {null, null}; 
		Contrainte c1;

		if (!this.estRealisable(P)) return null;
		c1 = this.estSurBord(P);
		if (c1 == null) return null;
		lesDeux[0] = c1;
		for (Contrainte c2 : contraintes) {
			if (c2 == c1) continue;
			if (c2.estSature(P)) {
				lesDeux[1] = c2;
				break;
			}
		}
		if (lesDeux[1] != null) return lesDeux;
		return null;
	}

	/**
	 * Recherche une intersection d'une demi-droite avec un bord quelconque du domaine ; si l'origine de la demi-droite
	 * est sur un des bords, elle n'est pas considérée comme intersection
	 * @param P0 l'origine de la demi-droite
	 * @param d la direction de la demi-droite
	 * @return s'il y a intersection, la valeur de t (t > 0) telle que PO + td soit sur un bord du domaine
	 *         sinon -1
	 */
	public double intersection(Couple P0, Couple d) {
		double t;
		Couple P;
		
		if (d == null) return 0;
		for (Contrainte c : contraintes) {
			if (c.estSature(P0)) continue;
			t = c.intersection(P0, d);
			if (t >= 0) {
				P = P0.ajoute(d.mult(t));
				if (this.estRealisable(P)) return t;
			}
		}
		return -1;
	}

	/**
	 * Calcule l'ensemble des bords du domaine
	 */
	private void calculerBordsDomaine() {
		for (Contrainte c : contraintes) {
			bords.add(new Bord(this, c));
		}
	}

	/**
	 * Supprime les contraintes qui ne contribuent pas à délimiter le domaine
	 */
	private void epurer() {
		Bord aRetire;

		do {
			aRetire = null;
			for (Bord b : bords)  {
				if (b.isVide()) {
					aRetire = b;
					break;
				}
			}
			if (aRetire != null) {
				bords.remove(aRetire);
				contraintes.remove(aRetire.getContrainte());			
			}
		}while (aRetire != null);
	}
	
	/**
	 * Calcule les coins du domaine ; chaque coin est un Couple contenant les coordonnées du coin
	 */
	private void calculerCoinsDomaine() {		
		Couple inter;

		for (Contrainte c1 : this.contraintes) 
			for (Contrainte c2 : this.contraintes) {
				inter = estCoin(c1, c2);		
				if (inter != null) coins.add(inter);
			}
	}

	/**
	 * Calcule le minimum et le maximum des abscisses du domaine ainsi que des ordonnées.
	 */
	private void calculerBornes() {
		ArrayList<Couple> coins = getCoins();
		Contrainte c;
		
		this.minorant_x = 0;
		this.majorant_x = 0;
		this.minorant_y = 0;
		this.majorant_y = 0;
		for (Couple coin : coins)  {	
			if (coin.x < minorant_x) this.minorant_x = coin.x;
			if (coin.x > majorant_x) this.majorant_x = coin.x;
			if (coin.y < minorant_y) this.minorant_y = coin.y;
			if (coin.y > majorant_y) this.majorant_y = coin.y;
		}
		for (Bord b : bords) {
			if ((b.isInfini()) && (b.getExtr()[0] == null)) {
				c = b.getContrainte();
				if (c.getCoeffx() != 0) {
					double valx = -c.getConstante() / c.getCoeffx();
					if (valx < minorant_x) this.minorant_x = valx;
					else if (valx > majorant_x) this.majorant_x = valx;
				}
				if (c.getCoeffy() != 0) {
					double valy = -c.getConstante() / c.getCoeffy();
					if (valy < minorant_y) this.minorant_y = valy;
					else if (valy > majorant_y) this.majorant_y = valy;
				}
			}
		}
	}

	@Override
	public String toString() {
		String chaine = "";
		for (Contrainte c : contraintes) chaine += c + "\n";
		return chaine;
	}
	
	/**
	 * @return la liste des contraintes du domaine, après suppression des contraintes inutiles
	 */
	public ArrayList<Contrainte> getContraintes() {
		return contraintes;
	}	
	
	/**
	 * @return l'ArrayList des bord du domaine
	 */
	public ArrayList<Bord> getBords() {
		return bords;
	}
	

	/**
	 * @return une ArrayList contenant les coins du domaine ; chaque coin est un Couple contenant les coordonnées du coin
	 */
	public ArrayList<Couple> getCoins()  {
		return coins;
	}
	
	/**
	 * Permet d'indiquer que le domaine est vide
	 */
	public void setVide() {
		vide = true;
	}
	
	/**
	 * @return truesi le domaine est vide, false sinon
	 */
	public boolean isVide() {
		return vide;
	}

	/**
	 * @return la plus grande abscisse des points du domaine
	 */
	public double getMajorant_x() {
		return majorant_x;
	}
	
	/**
	 * @return la plus grande ordonnée des points du domaine
	 */
	public double getMajorant_y() {
		return majorant_y;
	}

	/**
	 * @return la plus petite abscisse des points du domaine
	 */
	public double getMinorant_x() {
		return minorant_x;
	}

	/**
	 * @return la plus petite ordonnée des points du domaine
	 */
	public double getMinorant_y() {
		return minorant_y;
	}
}
