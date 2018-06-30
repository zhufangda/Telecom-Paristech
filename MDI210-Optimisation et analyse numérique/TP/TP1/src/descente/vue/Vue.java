package descente.vue;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Point;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import descente.modele.Couple;
import descente.modele.Descente;

public class Vue extends JPanel implements Observer {
	private static final long serialVersionUID = 1L;
	protected PanneauTrace panneauTrace;
	protected PanneauGadgets panneauGadgets;
	protected Descente descente;
	protected JScrollPane ascenseurs;
	protected Thread thread;
	protected boolean  aDebute;
	protected boolean pbPret;
	
	 public Vue( ) {
		this.setLayout(new BorderLayout());
		this.panneauTrace = new PanneauTrace(this);
		ascenseurs = new JScrollPane(this.panneauTrace);
		ascenseurs.setPreferredSize(panneauTrace.dimVue);
		this.centrerFenetre(); 
		this.add(ascenseurs, BorderLayout.CENTER);
		
		panneauGadgets = new PanneauGadgets(this);
		this.add(panneauGadgets, BorderLayout.EAST);
		setBorder(BorderFactory.createLineBorder(Color.BLACK));
	}
	
	protected void centrerFenetre() {
		ascenseurs.getViewport().setViewPosition(new Point((this.panneauTrace.largeur - this.panneauTrace.dimVue.width)/2, 
				(panneauTrace.hauteur - this.panneauTrace.dimVue.height)/2));	
	}

	@Override
	public void update(Observable o, Object arg) {
		Couple P = this.descente.getP();
		if (P != null) {
			Couple PCourt = P.tronquer(5);
			this.panneauGadgets.valeurX.setText(new Double(PCourt.x).toString());
			this.panneauGadgets.valeurY.setText(new Double(PCourt.y).toString());
			this.KuhnTucker();
		}
		this.panneauGadgets.nbPas.setText(new Integer(this.descente.getNbPas()).toString()); 
		if (!this.descente.atteintBorneInferieure()) {
			this.panneauGadgets.indication.setText("Le pb n'atteint pas de minimum");
			panneauTrace.repaint();
			this.panneauGadgets.stopper.setEnabled(false);
			this.panneauGadgets.reprendre.setEnabled(false);
			this.panneauGadgets.suspendre.setEnabled(false);
			this.pbPret = false;
			return;
		}
		panneauTrace.lesP.add(P);
		boolean change = false;
		if (P.x < panneauTrace.xMin) {
			panneauTrace.xMin = P.x;
			change = true;
		}
		else if (P.x > panneauTrace.xMax) {
			panneauTrace.xMax = P.x;
			change = true;
		}
		if (P.y < panneauTrace.yMin) {
			panneauTrace.yMin = P.y;
			change = true;
		}
		else if (P.y > panneauTrace.yMax) {
			panneauTrace.yMax = P.y;
			change = true;
		}
		if (change) {
			panneauTrace.ajusterDimensionVue(P);
			this.panneauTrace.centrer();
		}
		if (this.descente.isFinie()) {
			if (this.descente.isStoppee()) this.panneauGadgets.indication.setText("La descente est stoppee"); 
			else {
				double val = Couple.tronquer(this.descente.getPb().f(descente.getP()), 3);
				this.panneauGadgets.indication.setText("Le minimum de f vaut " + val);
				this.panneauGadgets.indicationSuite.setText("pour x = " + descente.getP().tronquer(4).x 
						+ " et y = " + descente.getP().tronquer(4).y);
			}
			this.panneauGadgets.stopper.setEnabled(false);
			this.panneauGadgets.reprendre.setEnabled(false);
			this.panneauGadgets.suspendre.setEnabled(false);
			this.panneauGadgets.validerPb.setEnabled(true);
		}	
		pbPret = false;
		panneauTrace.repaint();
	}


	protected void KuhnTucker() {
		String chaine1 = "Kuhn et Tucker : ";
		String chaine2;
		if (this.descente == null) return;
		Couple lesMu = this.descente.KuhnTucker(this.descente.getP());
		if (lesMu == null) {
			chaine1 += "non satisfaite"; 
			chaine2 = " ";
		}
		else {
			if (lesMu.x == 0.0) {
				chaine1 += "satisfaite a l'interieur";
				chaine2 = "gradient inferieur au seuil";
			}
			else if (lesMu.y == 0.0) {
				chaine1 += "satisfaite sur un bord";
				chaine2 = "(mu = " +  Couple.tronquer(lesMu.x, 2) + ")" ;
			}
			else {
				chaine1 += "satisfaite sur un coin";
				chaine2 = "(mu1 = " +  Couple.tronquer(lesMu.x, 2) + ", mu2 = " + Couple.tronquer(lesMu.y, 2) + ")" ;
			}
		}
		this.panneauGadgets.lesMu1.setText(chaine1);
		this.panneauGadgets.lesMu2.setText(chaine2);
	}

	protected void setDescente(Descente descente) {
		this.descente = descente;
		this.panneauTrace.setDescente(descente);
	}
}
