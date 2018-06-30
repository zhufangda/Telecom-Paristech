package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb4 extends Pb {
	public Pb4() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(-1, 0, 1));
		contraintes.add(new Contrainte(0, -1, 1));
		contraintes.add(new Contrainte(1, 1, -4));
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return  -Math.log(P.x + 2 * P.y);
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		valx = -1 / (P.x + 2 * P.y);
		valy = -2 / (P.x + 2 * P.y);
		return new Couple(valx, valy);
	}
	
	public String toString() {
		String chaine = "f = -ln(x + 2 * y)\n" +super.toString();
		return chaine;
	}

}
