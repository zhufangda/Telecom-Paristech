package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb10 extends Pb {
	public Pb10() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return 1 / Math.sqrt(P.x * P.x + P.y * P.y);   
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		valx = -P.x * Math.pow(P.x * P.x + P.y * P.y, -1.5);
		valy = -P.y * Math.pow(P.x * P.x + P.y * P.y, -1.5) / 2;
		return new Couple(valx, valy);
	}

	public String toString() {
		String chaine = "f = 1 / sqrt(x^2 + y^2 )\n" + super.toString();
		return chaine;
	}
}