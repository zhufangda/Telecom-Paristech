package descente.vue;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class PanneauGadgets extends Box {
	private static final long serialVersionUID = 1L;
	protected JButton demarrer = new JButton("Demarrer");
	protected JButton suspendre = new JButton("Suspendre");
	protected JButton reprendre = new JButton("Reprendre");
	protected JButton stopper = new JButton("Stopper");
	protected JTextField valeurX = new JTextField(10);
	protected JTextField valeurY = new JTextField(10);
	protected JButton validerPb  = new JButton("Valider probleme");
	protected JLabel  indication = new JLabel("Choisissez le probleme");
	protected JLabel  indicationSuite = new JLabel("              ");
	protected JTextField choixPb = new JTextField(4);
	protected JButton plus = new JButton("+");
	protected JButton moins = new JButton("-");
	protected JButton centrer = new JButton("Centrer");
	protected JTextField longueurMinGradient = new JTextField(new Double(0.00001).toString());
	protected JTextArea vueProbleme = new JTextArea(7, 20);
	protected JLabel lesMu1 = new JLabel("  ");
	protected JLabel lesMu2 = new JLabel("  ");
	protected JLabel nbPas = new JLabel("0");
	protected JTextField delai = new JTextField(7);
	protected Vue vue;

	protected PanneauGadgets(Vue vue){
		super(BoxLayout.Y_AXIS);
		JPanel panneau;
		JPanel grille;
		ControleurGadgets controleurGadgets = new ControleurGadgets(vue);

		this.vue = vue;
		
		this.add(Box.createVerticalStrut(10));
		
		grille = new JPanel(new GridLayout(2, 1));
		this.indication.setForeground(Color.RED);
		this.indicationSuite.setForeground(Color.RED);
		String nom = this.indication.getFont().getFontName();
		this.indication.setFont(new Font(nom,Font.BOLD, 14));
		this.indicationSuite.setFont(new Font(nom,Font.BOLD, 14));
		grille.add(this.indication);
		grille.add(indicationSuite);
		this.add(grille);	

		grille = new JPanel(new GridLayout(2, 1));
		panneau = new JPanel();
		panneau.add(new JLabel("Choix du probleme (1, 2, ...)"));
		choixPb.setText("1");
		choixPb.setActionCommand("validerPb");
		panneau.add(this.choixPb);
		choixPb.addActionListener(controleurGadgets);
		grille.add(panneau);

		panneau = new JPanel();
		this.validerPb.setActionCommand("validerPb");
		this.validerPb.addActionListener(controleurGadgets);
		panneau.add(this.validerPb);
		grille.add(panneau);
		
		grille.setBorder(BorderFactory.createLineBorder(Color.BLACK));
		add(grille);

		this.add(Box.createVerticalStrut(20));
		
		grille = new JPanel(new GridLayout(2, 1));
		
		panneau = new JPanel();
		panneau.add(new JLabel("abscise = "));
		this.valeurX.setText("0");
		this.valeurX.setActionCommand("position");
		this.valeurX.addActionListener(controleurGadgets);
		panneau.add(this.valeurX);
		grille.add(panneau);
		
		panneau = new JPanel();
		panneau.add(new JLabel("ordonnee = "));
		this.valeurY.setText("0");
		this.valeurY.setActionCommand("position");
		this.valeurY.addActionListener(controleurGadgets);
		panneau.add(this.valeurY);
		grille.add(panneau);	

		grille.setBorder(BorderFactory.createLineBorder(Color.BLACK));
		add(grille);

		this.add(Box.createVerticalStrut(10));

		this.demarrer.setActionCommand("demarrer");
		this.demarrer.addActionListener(controleurGadgets);
		this.add(demarrer);

		this.add(Box.createVerticalStrut(10));
		suspendre.setActionCommand("suspendre");
		add(suspendre);
		suspendre.addActionListener(controleurGadgets);
		suspendre.setEnabled(false);

		this.add(Box.createVerticalStrut(10));
		this.reprendre.setActionCommand("reprendre");
		this.reprendre.setEnabled(false);
		this.reprendre.addActionListener(controleurGadgets);
		add(this.reprendre);	

		this.add(Box.createVerticalStrut(10));
		this.stopper.setActionCommand("stopper");
		this.stopper.setEnabled(false);
		this.stopper.addActionListener(controleurGadgets);
		this.add(stopper);	

		this.add(Box.createVerticalStrut(10));

		panneau = new JPanel();
		this.vueProbleme.setEditable(false);
		JScrollPane ascenseurs = new JScrollPane(this.vueProbleme);
		panneau.add(ascenseurs);
		this.add(panneau);
		
		panneau = new JPanel();
		
		this.plus.setActionCommand("plus");
		this.plus.addActionListener(controleurGadgets);
		panneau.add(this.plus);

		this.moins.setActionCommand("moins");
		this.moins.addActionListener(controleurGadgets);
		panneau.add(this.moins);	

		this.centrer.setActionCommand("centrer");
		this.centrer.addActionListener(controleurGadgets);
		panneau.add(centrer);
		this.add(panneau);

		this.add(Box.createVerticalGlue());

		panneau = new JPanel();
		panneau.add(new JLabel("longueur minimum du gradient"));
		panneau.add(longueurMinGradient);
		this.add(panneau);	
		
		this.add(Box.createVerticalGlue());
		
		panneau = new JPanel();
		this.delai.setText("2000");
		this.delai.setActionCommand("delai");
		this.delai.addActionListener(controleurGadgets);
		panneau.add(new JLabel("delai en ms (entrer) : "));
		panneau.add(this.delai);
		this.add(panneau);

		this.add(Box.createVerticalStrut(5));
		
		panneau = new JPanel();
		panneau.add(new JLabel("nombre de pas : "));
		panneau.add(this.nbPas);
		this.add(panneau);

		panneau = new JPanel(new GridLayout(2, 1));
		panneau.add(lesMu1);
		panneau.add(lesMu2);
		this.add(panneau);

		this.add(Box.createVerticalStrut(5));	
	}
}
