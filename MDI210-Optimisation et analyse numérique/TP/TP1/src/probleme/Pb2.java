package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb2 extends Pb {
	public Pb2() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return  P.x * P.x +  P.y * P.y * P.y * P.y;
	}

	public Couple gradientf(Couple P) {
		double valx, valy;

		valx = 2 * P.x;
		valy = 4* P.y * P.y * P.y;
		return new Couple(valx, valy);
	}
	

	public String toString() {
		String chaine = "f = x^2 + 2 * y^4\n" +super.toString();
		return chaine;
	}
}

