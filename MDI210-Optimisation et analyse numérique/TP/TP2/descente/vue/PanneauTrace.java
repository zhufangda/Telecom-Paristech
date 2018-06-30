package descente.vue;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.util.ArrayList;

import javax.swing.JPanel;

import descente.modele.Bord;
import descente.modele.Contrainte;
import descente.modele.Couple;
import descente.modele.Descente;
import descente.modele.Domaine;

/**
 * Modélise le panneau dans lequel se font les tracés
 */
public class PanneauTrace extends JPanel {	
	protected static final long serialVersionUID = 1L;
	protected int largeur = 5000, hauteur = 5000;
	protected Dimension dim = new Dimension(largeur, hauteur);
	protected Domaine domaine;
	protected double margeReel = 0;
	protected int margePanneauX ;
	protected int margePanneauY ;
	protected Vue vue;
	protected ArrayList<Couple> coins;
	protected Domaine domaineVue;
	protected ArrayList<Couple> lesP = new ArrayList<Couple>(); 
	protected Dimension dimVue = new Dimension(800, 800);
	protected Couple copieDirection;
	protected double xMin = 0, xMax = 0, yMin = 0, yMax = 0;
	private boolean dedans;
	
	protected PanneauTrace(Vue vue) {
		this.vue = vue;
		margePanneauX = (this.largeur - (int)dimVue.getWidth()) / 2 + 50;
		margePanneauY = (this.hauteur - (int)dimVue.getHeight()) / 2  + 50;
		ControleurPanneauTrace controleurSouris = new ControleurPanneauTrace(vue);
		addMouseListener(controleurSouris);
		addMouseMotionListener(controleurSouris);
		setPreferredSize(dim);
	}
	
	protected void ajusterDimensionVue(Couple P) {	
		if (P != null) {
			if (P.x < this.xMin) xMin = P.x;
			else if (P.x > this.xMax) xMax = P.x;
			if (P.y < this.yMin) yMin = P.y;
			else if (P.y > this.yMax) yMax = P.y;
		}
		this.orthonormer();
	}
	protected void orthonormer() {
		if ((this.hauteur - 2 * this.margePanneauY) *(this.xMax - this.xMin) > 
		(this.largeur - 2 * this.margePanneauX) * (this.yMax - this.yMin)) 
			this.yMax = (this.hauteur - 2 * this.margePanneauY) * (this.xMax - this.xMin) / (this.largeur - 2 * this.margePanneauX) + this.yMin;
		else this.xMax = (this.largeur - 2 * this.margePanneauX) * (this.yMax - this.yMin) / (this.hauteur - 2 * this.margePanneauY) + this.xMin;
	}
	
	// transforme l'équation d'une contrainte dans les mesures de la fenêtre (repère Vue)
	protected Contrainte contrainteVue(Contrainte c)  {
		double coeffxVue, coeffyVue, constanteVue;
		
		double facteurx = (this.xMax - this.xMin) / (this.dim.width - 2 * this.margePanneauX);
		double facteury = (this.yMax - this.yMin) / (this.dim.height - 2 * this.margePanneauY);
		coeffxVue = c.getCoeffx() * facteurx;
		coeffyVue = -c.getCoeffy() * facteury;
		constanteVue = c.getCoeffx() * (this.xMin - this.margePanneauX * facteurx) +
		c.getCoeffy() * (this.yMin + (this.dim.height - this.margePanneauY) * facteury) + c.getConstante();
		
		return new Contrainte(coeffxVue, coeffyVue, constanteVue);	
	}
		
	// Convertit les coordonnées de P dans la repère Vue
	protected Couple convertir(Couple P) {
		if (P == null) return null;
		double abs = (this.dim.width - 2 * this.margePanneauX) * (P.x - this.xMin) / (this.xMax - this.xMin) + this.margePanneauX;
		double ord = this.dim.height + (2 * this.margePanneauY - this.dim.height) * (P.y - this.yMin) / (this.yMax - this.yMin) - this.margePanneauY;
		
		return new Couple(abs, ord);
	}
	
	// Inverse de la précédente
	protected Couple inverseConvertir(Couple P) {
		if (P == null) return null;
		double abs = this.xMin + (P.x - this.margePanneauX) * (this.xMax - this.xMin) / (this.dim.width - 2 * this.margePanneauX);
		double ord = this.yMin + (P.y + this.margePanneauY - this.dim.height) * (this.yMax - this.yMin)/(2 * this.margePanneauY - this.dim.height);
		return new Couple(abs, ord);
	}

	// Convertit un vecteur dans le repère Vue
	protected Couple sortie(Couple P, Couple d) {
		Couple dSym = new Couple(d.x * (this.largeur - 2 * this.margePanneauX) / (this.xMax - this.xMin) , 
				-d.y * (this.hauteur - 2 * this.margePanneauY) / (this.yMax - this.yMin));
		double t = domaineVue.intersection(P, dSym);
		return P.ajoute(dSym.mult(t));
	}

	protected void dessinerVecteur(Couple origine, Couple d, Graphics2D g, boolean normer) {
		g.setStroke(new BasicStroke(3));
		if (d == null) return;
		Couple diagonale = new Couple(this.xMax - this.xMin, this.yMax - this.yMin);
		
		//Les trois lignes suivantes servent à normer
		double longueurDiagonale = diagonale.norme();
		double norme = d.norme();
		if (normer) d = d.mult(longueurDiagonale/(10 * norme));
		
		// Calcul du vecteur dans le repère Vue
		Couple extr = origine.ajoute(d);
		Couple origineVue = this.convertir(origine);
		Couple extrVue = this.convertir(extr);

		Couple v = extrVue.ajoute(origineVue.mult(-1));
		if (v.norme() < 5) g.drawLine((int) origineVue.x, (int) origineVue.y, (int) extrVue.x, (int) extrVue.y);	
		//Dessin du vecteur
		else this.drawArrow(g, (int) origineVue.x, (int) origineVue.y, (int) extrVue.x, (int) extrVue.y);	

		g.setStroke(new BasicStroke(1));
	}

	// Dessine le trajet
	protected void dessinerLesP(Graphics2D g) {
		g.setColor(Color.RED);
		g.setStroke(new BasicStroke(3));
		if (lesP.size() < 2)return;
		Couple XVue1 = this.convertir(lesP.get(0));
		Couple XVue2;
		
		for (int i = 1; i < lesP.size(); i++) {
			XVue2 = this.convertir(lesP.get(i));
			g.drawLine((int)XVue1.x, (int)XVue1.y, (int)XVue2.x, (int)XVue2.y);
			XVue1 = XVue2;
			if (i == lesP.size() - 1) g.setColor(Color.BLACK);
			g.fillOval((int)XVue1.x - 5, (int)XVue1.y - 5, 10, 10);
		}
		g.setStroke(new BasicStroke(1));
	}
	
	protected void dessinerAxes(Graphics2D g) {
		Couple p1;
		Couple p2;
		Couple d;
		Couple origine = new Couple(0, 0);
		
		g.setColor(Color.GRAY);
		p1 = this.convertir(origine);
		d = new Couple(0, 1);
		p2 = this.sortie(p1, d);
		this.drawArrow(g, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
		d = new Couple(1, 0);
		p2 = this.sortie(p1, d);
		this.drawArrow(g, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
		d = new Couple(0, -1);
		p2 = this.sortie(p1, d);
		g.drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
		d = new Couple(-1, 0);
		p2 = this.sortie(p1, d);
		g.drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
		g.setColor(Color.BLACK);
	}
	
	protected void dessinerBord(Bord b, Graphics2D g) {
		Couple p1 = null;
		Couple p2 = null;
		Couple d;
		
		if (!b.isInfini()) {
			p1 = this.convertir(b.getExtr()[0]);
			p2 = this.convertir(b.getExtr()[1]);
		}
		else {
			if (b.getExtr()[0] != null) {
				p1 = this.convertir(b.getExtr()[0]);
				d = b.getD();
				p2 = this.sortie(p1, d);
			}
			else {
				Couple inter;
				Contrainte c = b.getContrainte();
				Contrainte cVue = this.contrainteVue(c);
				
				inter = this.domaineVue.getContraintes().get(0).intersection(cVue);
				if (inter != null) {
					p1 = inter;
					p2 = this.domaineVue.getContraintes().get(2).intersection(cVue);
				}
				else {
					p1 = this.domaineVue.getContraintes().get(1).intersection(cVue);
					p2 = this.domaineVue.getContraintes().get(3).intersection(cVue);
					
				}
			}
		}
		g.drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
	}
	
	protected void dessinerDomaine(Graphics2D g) {	
		Couple p;
	
		if (this.domaine.isVide()) return;
		g.setStroke(new BasicStroke(2));
		for (Bord b : this.domaine.getBords()) {
			this.dessinerBord(b, g);
		}
		
		for (Couple coin : coins) {
			p = this.convertir(coin);		
			g.drawString(coin.tronquer(2).toString(), (int)p.x + 15,(int) p.y);
		}
		g.setStroke(new BasicStroke(1));
	}
	
	@Override
	protected void paintComponent(Graphics g1) {
		Couple direction;
		
		super.paintComponent(g1);
		Graphics2D g = (Graphics2D) g1;
		Descente descente = vue.descente;
		if (descente == null) return;
		Couple P  = descente.getP();
		if (P == null) {
			this.dessinerAxes(g);
			this.dessinerDomaine(g);
			if ((descente != null) && !descente.atteintBorneInferieure())  {
				this.dessinerLesP(g);	
				g.setColor(Color.BLUE);
				Couple dernierP = lesP.get(lesP.size() - 1);
				this.dessinerVecteur(dernierP, descente.getPb().gradientf(dernierP), g, false);
				g.setColor(Color.GREEN);
				this.dessinerVecteur(dernierP, copieDirection, g, true);
			}
			return;
		}
		this.dessinerAxes(g); 
		this.dessinerDomaine(g);
    	Couple PVue = this.convertir(P);
    	g.fillOval((int)PVue.x - 5, (int)PVue.y - 5, 10, 10);
    	if (!dedans) return;
		this.dessinerLesP(g);	
    	g.setColor(Color.BLUE);
		this.dessinerVecteur(P, descente.getPb().gradientf(P), g, false);
		g.setColor(Color.GREEN);
		if(descente.isFinie()) return;
		direction = descente.getDirection();
		this.dessinerVecteur(P, direction, g, true);
		copieDirection = direction;
	}

	protected void centrer() {
		Couple centreVue;
		if (this.vue.descente.getP() != null) centreVue = this.vue.panneauTrace.convertir(this.vue.descente.getP());
		else centreVue = this.vue.panneauTrace.convertir(new Couple((this.vue.panneauTrace.xMax + this.vue.panneauTrace.xMin)/2,
				(this.vue.panneauTrace.yMax + this.vue.panneauTrace.yMin)/2));
		Point point = new Point((int)centreVue.x - this.vue.panneauTrace.dimVue.width / 2, 
				(int)centreVue.y - this.vue.panneauTrace.dimVue.height / 2);
		this.vue.ascenseurs.getViewport().setViewPosition(point);
	}
	/**
	 * Permet de visualiser la position de depart de la descente
	 * @param PReel la position de depart
	 */
	protected void validerPosition(Couple PReel) {
		if (vue.descente == null) return;

		this.vue.descente.setP(PReel);
		if (!this.vue.descente.getPb().getDomaine().estRealisable(PReel)) {
			this.vue.panneauGadgets.indication.setText("Le point n'est pas dans le domaine");
			this.vue.panneauGadgets.indicationSuite.setText("Choisissez un autre point");
			this.dedans = false;
		}
		else {
			this.vue.panneauGadgets.indication.setText("Vous pouvez demarrer");
			this.vue.panneauGadgets.indicationSuite.setText("     ");
			this.vue.KuhnTucker();
			this.dedans = true;
		}
		Couple PCourt = PReel.tronquer(2);
		vue.panneauGadgets.valeurX.setText((Double.toString(PCourt.x)));
		vue.panneauGadgets.valeurY.setText(Double.toString(PCourt.y));
		vue.panneauTrace.repaint();
	}
	
	// Dessine une flèche de (x,y) à (xx, yy)
	protected void drawArrow(Graphics g, int x, int y, int xx, int yy ){
		float arrowWidth = 6.0f ;
		float theta = 0.423f ;
		int[] xPoints = new int[ 3 ],yPoints = new int[ 3 ] ;
		float[] vecLine = new float[ 2 ] ;
		float[] vecLeft = new float[ 2 ] ;
		float fLength;
		float th;
		float ta;
		float baseX, baseY ;
		
		xPoints[ 0 ] = xx ;
		yPoints[ 0 ] = yy ; // build the line vector
		vecLine[ 0 ] = (float)xPoints[ 0 ] - x ;
		vecLine[ 1 ] = (float)yPoints[ 0 ] - y ;
		// build the arrow base vector - normal to the line
		vecLeft[ 0 ] = -vecLine[ 1 ] ;
		vecLeft[ 1 ] = vecLine[ 0 ] ;
		// setup length parameters
		fLength = (float)Math.sqrt( vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1] ) ;
		th = arrowWidth / ( 2.0f * fLength ) ;
		ta = arrowWidth / ( 2.0f * ( (float)Math.tan( theta ) / 2.0f ) * fLength ) ;
		// find the base of the arrow
		baseX = ( (float)xPoints[ 0 ] - ta * vecLine[0]);
		baseY = ( (float)yPoints[ 0 ] - ta * vecLine[1]);
		// build the points on the sides of the arrow
		xPoints[1] = (int)( baseX + th * vecLeft[0] );
		yPoints[1] = (int)( baseY + th * vecLeft[1] );
		xPoints[2] = (int)( baseX - th * vecLeft[0] );
		yPoints[2] = (int)( baseY - th * vecLeft[1] );
		g.drawLine( x, y, (int)baseX, (int)baseY ) ;
		g.fillPolygon( xPoints, yPoints, 3 ) ;
	}

	protected void setDescente(Descente descente) {
		if (descente == null) return;
		ArrayList<Contrainte> contraintesVue = new ArrayList<Contrainte>(); 
		contraintesVue.add(new Contrainte(-1, 0, 0)); 
		contraintesVue.add(new Contrainte(0, -1, 0)); 
		contraintesVue.add(new Contrainte(1, 0, -this.largeur)); 
		contraintesVue.add(new Contrainte(0, 1, -this.hauteur));
		domaineVue = new Domaine(contraintesVue, false);
		this.domaine = descente.getPb().getDomaine();
		this.xMin = domaine.getMinorant_x();
		this.xMax = domaine.getMajorant_x();
		this.yMin = domaine.getMinorant_y();
		this.yMax = domaine.getMajorant_y();

		if (this.xMax - this.xMin < 1) {
			this.xMax += 2;
			this.xMin -= 2;
		}
		else {
			this.xMax += 0.1;
			this.xMin -= 0.1;
		}
		if (this.yMax - this.yMin < 1) {
			this.yMax += 2;
			this.yMin -= 2;
		}
		else {
			this.yMax += 0.1;
			this.yMin -= 0.1;
		}
		this.coins = domaine.getCoins();
		this.ajusterDimensionVue(null);
		this.repaint();
	}
}

