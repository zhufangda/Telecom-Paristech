import java.awt.*;

public class Main {

    public static void main(String[] args) {


        EventQueue.invokeLater(()->
        {
            ClientFrame frame = new ClientFrame("Title");
            frame.setVisible(true);

        });
    }
}
