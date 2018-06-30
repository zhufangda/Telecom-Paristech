package descente.modele;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Modélise un bord du domaine ; cela peut être une droite, une demi-droite, un segment, ou être vide
 *
 */
public class Bord {
	private Couple[] extr = new Couple[2]; // contient les deux extrémités quand il s'agit d'n segment, 
										   // l'extrémité si c'est une demi-droite
	private boolean infini = false;
	private Couple d = null; // s'il s'agit d'une demi-droite, la direction de cette demi-droite
	private boolean vide = false;
	private Contrainte contrainte; // la contrainte correspondant à ce bord

	public Bord(Domaine domaine, Contrainte c) {
		super();
		int nb = 0;
		this.contrainte = c;
		ArrayList<Contrainte> contraintes = domaine.getContraintes();
		Contrainte coupe = null;

		// calcul les intersections avec les autres droites
		for (Contrainte c1 : contraintes) {
			if (domaine.estCoin(c, c1) != null) {
				Couple inter = c.intersection(c1);
				if (nb == 0) coupe = c1;
				extr[nb] = inter;
				nb++;
			}
		}

		if (nb == 0) {
			infini = true;
			for (Contrainte c1 : contraintes) {
				if (c != c1) {
					int parallele = c.estParallele(c1);
					if (parallele == 0) {
						this.vide = true;
					}
					if (parallele == -2) {
						domaine.setVide();
					}
					if (parallele == -1) this.vide = true;				
				}
			}
		}

		if((nb == 2) && (Descente.estNul(extr[0].distance(this.extr[1])))) this.vide = true;	
		else if (nb == 1) {
			d = c.getBordUnitaire();
			if (d.produitScalaire(coupe.getGradient()) > 0) d = d.mult(-1);
			infini = true;
		}		
	}

	@Override
	public String toString() {
		return "Bord [d=" + this.d + ", extr=" + Arrays.toString(this.extr) + ", infini="
		+ infini + ", vide=" + this.vide + "], contrainte : " + this.contrainte;
	}

	/**
	 * @return true si le bord est une droite ou une demi-droite, false si c'est un segment
	 */
	public boolean isInfini() {
		return this.infini;
	}

	/**
	 * @return true si la contrainte ne limite pas le domaine, false sinon
	 */
	public boolean isVide() {
		return this.vide;
	}

	/**
	 * @return la direction unitaire de la demi-droite s'il s'agit d'une demi-droite, null dans les cas contraires
	 */
	public Couple getD() {
		return this.d;
	}

	/**
	 * @return - les deux extrémités quand il s'agit d'un segment, 
	 * <br>- l'extrémité dans la case d'indice 0 si c'est une demi-droite, null dans l'autre case
	 * <br>- s'il s'agit d'une droite, null dans les deux cases
	 */
	public Couple[] getExtr() {
		return extr;
	}
	
	/**
	 * @return la contrainte correspondant à ce bord
	 */
	public Contrainte getContrainte() {
		return contrainte;
	}
}
