package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb3 extends Pb {
	public Pb3() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(-1, 0, 0.2));
		contraintes.add(new Contrainte(0, -1, 0.2));
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return  -Math.log(P.x + 2 * P.y);
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		if (P == null) return null;
		valx = -1 / (P.x + 2 * P.y);
		valy = -2 / (P.x + 2 * P.y);
		return new Couple(valx, valy);
	}
	
	public String toString() {
		String chaine = "f = -ln(x + 2 * y)\n" +super.toString();
		return chaine;
	}

}
