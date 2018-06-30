package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb12 extends Pb {
	public Pb12() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(0, -1, 0));
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return  Math.pow(P.x - 5, 4) + Math.pow(P.y - 2, 4);  
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		valx = 4 * Math.pow(P.x - 5, 3);
		valy = 4 * Math.pow(P.y - 2, 3);
		return new Couple(valx, valy);
	}

	public String toString() {
		String chaine = "f = (x - 5) ^ 4 + (y - 2) ^ 4\n" + super.toString();
		return chaine;
	}
}