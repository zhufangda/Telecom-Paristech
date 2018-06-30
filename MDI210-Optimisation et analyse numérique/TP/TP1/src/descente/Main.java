package descente;

import javax.swing.JFrame;

import descente.vue.Vue;

public class Main {
	public static void main(String[] arg) {
		JFrame fenetre = new JFrame();
		fenetre.setContentPane(new Vue());
		fenetre.setLocation(300, 100);
		fenetre.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		fenetre.pack();
		fenetre.setVisible(true);
	}
}
