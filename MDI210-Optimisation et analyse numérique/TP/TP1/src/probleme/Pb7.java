package probleme;

import java.util.ArrayList;

import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Domaine;


public class Pb7 extends Pb {
	public Pb7() {
		ArrayList<Contrainte> contraintes = new ArrayList<Contrainte>();
		contraintes.add(new Contrainte(-1, -1, 1));
		contraintes.add(new Contrainte(-1, 0, 0));
		domaine = new Domaine(contraintes, true);
		// this.setXInterne(new Couple(1, 1));
	}

	public double f(Couple P) {
		return P.x * P.x + + P.y * P.y + P.x * P.y - 4 *  P.x ;
	}
	
	public Couple gradientf(Couple P) {
		double valx, valy;
		
		valx = 2 * P.x + P.y - 4;
		valy = P.x + 2 * P.y;
		return new Couple(valx, valy);
	}

	public String toString() {
		String chaine = "f = x^2 + y^2 + x * y - 4x\n" +super.toString();
		return chaine;
	}
}

