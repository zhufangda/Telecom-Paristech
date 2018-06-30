import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.sql.Timestamp;
import java.util.concurrent.ExecutionException;

public class ClientFrame extends JFrame {
	
    private static final long serialVersionUID = 1L;

    private JButton btn_send;
    private JButton btn_clear;
    private JButton btn_exit;
    private JButton btn_play;
    private JButton btn_search;
    
    
    private JTextArea resultsArea; // Show the response of server
    private JTextField cmdArea; //  Area to type the cmd
    private String cmd;
    
    private static final int DEFAULT_WIDTH = 800;
    private static final int DEFAULT_HEIGHT = 600;

    private String host = "localhost";
    private int port = 3331;
    
    public ClientFrame(String title) throws HeadlessException {

        super(title);
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        setLayout(new BorderLayout());

        cmdArea = new JTextField();
        add(cmdArea, BorderLayout.NORTH);

        // Add Button Panel
        JPanel btnPanel = new JPanel();
        btnPanel.setLayout(new GridLayout(1,3));

        btn_play = new JButton("play");
        btn_play.addActionListener(new PlayAction());
        btn_play.setEnabled(true);
        
        btn_search = new JButton("search");
        btn_search.addActionListener(new SearchAction());
        btn_search.setEnabled(true);
             
        btn_send = new JButton("Send");
        btn_send.addActionListener(new SendAction());
        btn_send.setEnabled(true);


        btn_clear = new JButton("Clear");
        btn_clear.addActionListener(new ClearAction());
        btn_clear.setEnabled(true);

        btn_exit = new JButton("Exit");
        btn_exit.addActionListener(new ExitAction());
        btn_exit.setEnabled(true);

        btnPanel.add(btn_play);
        btnPanel.add(btn_search);
        btnPanel.add(btn_send);
        btnPanel.add(btn_clear);
        btnPanel.add(btn_exit);

        add(btnPanel, BorderLayout.SOUTH);

        resultsArea = new JTextArea();
        resultsArea.setEditable(false); 
        resultsArea.setForeground(Color.BLACK);
        JScrollPane scrollPane = new JScrollPane(resultsArea); 
        add(scrollPane, BorderLayout.CENTER);
        addText("test");

    }
    
    
   public void addText(String output) {
	   StringBuilder results = new StringBuilder(resultsArea.getText());
	   results.append(new Timestamp(System.currentTimeMillis()).toString())
	   .append("]")
	   .append(output)
	   .append('\n');
	   resultsArea.setText(results.toString());
   }
   class PlayAction extends  AbstractAction{

       /**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
       public void actionPerformed(ActionEvent e) {
       	cmd = "play " + cmdArea.getText();
       	cmdArea.setText("");
       	addText("Command:" + cmd);
       	Client client = null;
			try {
				client = new Client(cmd, host, port);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
       	client.execute();
       }
	
   } 
   
   class SearchAction extends  AbstractAction{

       /**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
       public void actionPerformed(ActionEvent e) {
       	addText("Command:" + cmd);
       	Client client = null;
			try {
				client = new Client("search file " + cmdArea.getText(), host, port);
				client.execute();
				client = new Client("search group " + cmdArea.getText(), host, port);
				client.execute();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
       	cmdArea.setText("");
       }
   }
   
    class SendAction extends  AbstractAction{

        /**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
        public void actionPerformed(ActionEvent e) {
        	cmd = cmdArea.getText();
        	cmdArea.setText("");
        	addText("Command:" + cmd);
        	Client client = null;
			try {
				client = new Client(cmd, host, port);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
        	client.execute();
        }
    }

    class ClearAction extends  AbstractAction{

        /**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
        public void actionPerformed(ActionEvent e) {
        	resultsArea.setText("");
        }
    }

    class ExitAction extends  AbstractAction{

        /**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
        public void actionPerformed(ActionEvent e) {
        		System.exit(0);
        }
    }
    
    
    
    
    class Client extends SwingWorker<String, String>
    {
    	static final String DEFAULT_HOST = "localhost";
    	static final int DEFAULT_PORT = 3331;
    	  
        private String request;
        private int port;
        private String host;
        private Socket socket = null;;
        private BufferedReader input;
        private BufferedWriter output;
        
        public Client(String request, String host, int port) throws IOException {
        	this.request = request;
        	try {
        	      socket = new java.net.Socket(host, port);
        	    }
        	    catch (java.net.UnknownHostException e) {
        	      System.err.println("Client: Couldn't find host "+host+":"+port);
        	      throw e;
        	    }
        	
        	    catch (java.io.IOException e) {
        	      System.err.println("Client: Couldn't reach host "+host+":"+port);
        	      throw e;
        	    }
        	    
        	    try {
        	      input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        	      output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        	    }
        	    catch (java.io.IOException e) {
        	      System.err.println("Client: Couldn't open input or output streams");
        	      throw e;
        	    }

        }
        
        protected String doInBackground() throws UnknownHostException, IOException, InterruptedException, ExecutionException
        {
        	 try {
        	      request += "\n";  // ajouter le separateur de lignes
        	      output.write(request, 0, request.length());
        	      output.flush();
        	    }
        	    catch (java.io.IOException e) {
        	      System.err.println("Client: Couldn't send message: " + e);
        	      return null;
        	    }
        	    
        	    // Recuperer le resultat envoye par le serveur
        	 
        	    try {
        	    	StringBuilder builder = new StringBuilder();
        	      	String[] list = input.readLine().split(",");
          			for(int i = 0; i<list.length; i++) {
          				builder.append(list[i]).append("\n");
          			}

      				return builder.toString();
        	    }catch (java.io.IOException e) {
        	      System.err.println("Client: Couldn't receive message: " + e);
        	      return null;
        	    } 
        	  }

        protected void done()
        {

            	try {
            		
					addText(get());
				} catch (InterruptedException | ExecutionException e) {
					// TODO Auto-generated catch block
					addText("Server Error:" + e.getMessage());
				}


        }

    }
}



