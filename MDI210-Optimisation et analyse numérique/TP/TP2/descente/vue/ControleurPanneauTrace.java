package descente.vue;

import java.awt.Cursor;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import descente.modele.Couple;

public class ControleurPanneauTrace extends MouseAdapter {
	private Vue vue;
	private Couple PDepart;

	protected ControleurPanneauTrace(Vue vue) {
		this.vue = vue;
	}

	@Override
	public void mouseClicked(MouseEvent evt) {
		if ((this.vue.thread != null) && this.vue.thread.isAlive()) return;
		this.vue.panneauTrace.lesP.clear();
		Couple P = new Couple(evt.getX(), evt.getY());
		Couple PReel = vue.panneauTrace.inverseConvertir(P);
		this.vue.panneauTrace.validerPosition(PReel);
	}

	@Override
	public void mouseEntered(MouseEvent evt)  {
		this.vue.panneauTrace.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
	}
	

	@Override
	public void mouseExited(MouseEvent evt)  {
		this.vue.panneauTrace.setCursor(Cursor.getDefaultCursor());
	}

	@Override
	public void mousePressed(MouseEvent evt) {
		PDepart = new Couple(evt.getX(), evt.getY());
	}
	@Override
	public void mouseReleased(MouseEvent evt) {
		PDepart = null;
	}
	
	@Override
	 public void mouseDragged(MouseEvent evt) {
		 if (PDepart != null) {
				Couple PActuel = new Couple(evt.getX(), evt.getY());
				Couple difference = PActuel.soustrait(PDepart);
				Point anciennePos = vue.ascenseurs.getViewport().getViewPosition();
				int nouvellePosX = anciennePos.x - (int)difference.x;
				int nouvellePosY = anciennePos.y - (int)difference.y;
				vue.ascenseurs.getViewport().setViewPosition(new Point(nouvellePosX, nouvellePosY));
				vue.panneauTrace.repaint();
		 }
	 }      
}
