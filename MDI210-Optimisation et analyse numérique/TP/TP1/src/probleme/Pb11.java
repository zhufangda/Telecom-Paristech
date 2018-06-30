package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb11 extends Pb {
	public Pb11() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(0, -1, 0));
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return  P.x + P.y;   
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		valx = 1;
		valy = 1;
		return new Couple(valx, valy);
	}

	public String toString() {
		String chaine = "f = x + y\n" + super.toString();
		return chaine;
	}
}