package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb8 extends Pb {
	public Pb8() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(-1, 0, 1));
		contraintes.add(new Contrainte(0, -1, 0));
		contraintes.add(new Contrainte(-1, -1, 4));
		domaine = new Domaine(contraintes, true);
	}

	public double f(Couple P) {
		return P.x * P.x * P.x + 4 *P.y * P.y - P.x * P.y;   
	}
	
	public Couple gradientf(Couple P) {
		double valx, valy;
		
		valx = 3 * P.x * P.x - P.y;
		valy = 8 * P.y + P.x ;
		return new Couple(valx, valy);
	}

	public String toString() {
		String chaine = "f = x^3 + y^2 - x * y\n" +super.toString();
		return chaine;
	}
}
