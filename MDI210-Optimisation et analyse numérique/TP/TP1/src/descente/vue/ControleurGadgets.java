package descente.vue;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import probleme.Pb;

import descente.modele.Couple;
import descente.modele.Descente;

 public class ControleurGadgets implements ActionListener {
	private Vue vue;
	protected ControleurGadgets (Vue vue) {
		this.vue = vue;
	}

	public void actionPerformed(ActionEvent evt) {
		String commande = evt.getActionCommand();

		if (commande.equals("demarrer")) {	
			if ((this.vue.thread != null) && this.vue.thread.isAlive()) return;
			if (this.vue.aDebute) this.reinitialiser();
			if (!this.vue.pbPret) this.validerPb();
			Couple PReel = new Couple(new Double(this.vue.panneauGadgets.valeurX.getText()),
					new Double(this.vue.panneauGadgets.valeurY.getText()));

			if (!this.vue.descente.getPb().getDomaine().estRealisable(PReel)) {
				this.vue.panneauGadgets.indication.setText("Le point n'est pas dans le domaine");
				this.vue.panneauGadgets.indicationSuite.setText("Choisissez un autre point");
				return;
			}
			this.vue.descente.setP(PReel);
			this.vue.panneauTrace.lesP.add(PReel);
			this.vue.descente.setDelai(Integer.parseInt(this.vue.panneauGadgets.delai.getText()));
			this.vue.panneauTrace.ajusterDimensionVue(PReel);
			this.vue.centrerFenetre();
			this.vue.panneauTrace.repaint();
			this.vue.panneauTrace.repaint();

			this.vue.panneauGadgets.suspendre.setEnabled(true);
			this.vue.panneauGadgets.stopper.setEnabled(true);
			this.vue.panneauGadgets.indication.setText("    ");
			this.vue.panneauGadgets.indicationSuite.setText("    ");
			this.vue.descente.setSeuil(new Double(this.vue.panneauGadgets.longueurMinGradient.getText()));
			this.vue.aDebute = true;
			this.vue.thread = new Thread(this.vue.descente);
			this.vue.thread.start();
		}

		else if (commande.equals("position")) {
			if ((this.vue.thread != null) && this.vue.thread.isAlive()) return;
			this.vue.panneauTrace.lesP.clear();
			Couple PReel = new Couple(new Double(this.vue.panneauGadgets.valeurX.getText()),
					new Double(this.vue.panneauGadgets.valeurY.getText()));
			this.vue.panneauTrace.validerPosition(PReel);
		}

		else if (commande.equals("suspendre")) {
			if ((this.vue.thread == null) || !this.vue.thread.isAlive()) return;
			this.vue.descente.setSuspendre(true);
			this.vue.panneauGadgets.reprendre.setEnabled(true);
			this.vue.panneauGadgets.stopper.setEnabled(false);
		}
		else if (commande.equals("reprendre")) {
			if ((this.vue.thread == null) || !this.vue.thread.isAlive()) return;
			this.vue.descente.setSuspendre(false);
			this.vue.panneauGadgets.stopper.setEnabled(true);
			synchronized(this.vue.descente) {
				this.vue.descente.notifyAll();
			}
		}
		else if (commande.equals("stopper")) {
			if ((this.vue.thread == null) || !this.vue.thread.isAlive()) return;				
			this.vue.descente.stopper();	
		}

		else if (commande.equals("validerPb")) {
			if ((this.vue.thread != null) && this.vue.thread.isAlive()) return;
			validerPb();
		}
		else if (commande.equals("moins")) {
			if (this.vue.descente == null) return;
			double dx = this.vue.panneauTrace.xMax - this.vue.panneauTrace.xMin;
			double dy = this.vue.panneauTrace.yMax - this.vue.panneauTrace.yMin;
			this.vue.panneauTrace.xMin -= dx/4;
			this.vue.panneauTrace.xMax += dx/4;
			this.vue.panneauTrace.yMin -= dy/4;
			this.vue.panneauTrace.yMax += dy/4;
			this.vue.panneauTrace.orthonormer();
			this.vue.panneauTrace.centrer();
			this.vue.panneauTrace.repaint();
		}

		else if (commande.equals("plus")) {
			if (this.vue.descente == null) return;
			double dx = this.vue.panneauTrace.xMax - this.vue.panneauTrace.xMin;
			double dy = this.vue.panneauTrace.yMax - this.vue.panneauTrace.yMin;
			this.vue.panneauTrace.xMin += dx/8;
			this.vue.panneauTrace.xMax -= dx / 8;
			this.vue.panneauTrace.yMin += dy/8;
			this.vue.panneauTrace.yMax -= dy/8;
			this.vue.panneauTrace.orthonormer();
			this.vue.panneauTrace.centrer();
			this.vue.panneauTrace.repaint();
		}


		else if (commande.equals("centrer")) {
			if (this.vue.descente == null) return;
			this.vue.panneauTrace.centrer();
			this.vue.panneauTrace.repaint();
		}

		else if (commande.equals("delai")) {
			if (this.vue.descente != null) this.vue.descente.setDelai(Integer.parseInt(this.vue.panneauGadgets.delai.getText()));
		}
	}


	protected void reinitialiser() {
		this.vue.setDescente(null);
		this.vue.panneauTrace.lesP.clear();
		this.vue.panneauGadgets.indication.setText("Choisissez le probleme");
		this.vue.panneauGadgets.indicationSuite.setText("     ");
		this.vue.panneauGadgets.nbPas.setText("0");
		this.vue.ascenseurs.getViewport().setViewPosition(new Point((this.vue.panneauTrace.largeur - this.vue.panneauTrace.dimVue.width)/2, 
				(this.vue.panneauTrace.hauteur - this.vue.panneauTrace.dimVue.height)/2));
		this.vue.aDebute = false;
		this.vue.panneauTrace.xMin = this.vue.panneauTrace.xMax = this.vue.panneauTrace.yMin = this.vue.panneauTrace.yMax = 0;
		this.vue.panneauTrace.repaint();
	}

	protected void validerPb() {
		if (this.vue.aDebute) reinitialiser();
		try {
			Pb pb = choixPb();
			this.vue.setDescente(new Descente(pb));
			if (pb.getDomaine().isVide()) {
				this.vue.panneauGadgets.indication.setText("Le domaine est vide");
				this.vue.panneauGadgets.indicationSuite.setText("Choisissez un autre probleme");
				return;
			}
			this.vue.panneauGadgets.indication.setText("Choisissez le point de départ");
			this.vue.panneauGadgets.indicationSuite.setText("En cliquant ou directement");
			this.vue.panneauTrace.repaint();
			this.vue.panneauGadgets.vueProbleme.setText(pb.toString());
			this.vue.descente.addObserver(this.vue);
			this.vue.pbPret = true;
		}
		catch(ClassNotFoundException exc) {
			vue.panneauGadgets.indication.setText("Probleme inexistant");
			vue.panneauGadgets.indicationSuite.setText("Choisir un autre numero");
		}
		catch(IllegalAccessException exc) {
			vue.panneauGadgets.indication.setText("Probleme d'acces illegal");
			vue.panneauGadgets.indicationSuite.setText("Choisir un autre numero");
		}
		catch(InstantiationException exc) {
			vue.panneauGadgets.indication.setText("Probleme d'instantiation");
			vue.panneauGadgets.indicationSuite.setText("Choisir un autre numero");
		}
	}

	protected Pb choixPb() throws ClassNotFoundException, IllegalAccessException, InstantiationException {
		Class<?> classe;	
		Pb pb = null;	
		String nom = "probleme.Pb" + this.vue.panneauGadgets.choixPb.getText();

		classe = Class.forName(nom);
		pb = (Pb) classe.newInstance();
		return pb;
	}
}
