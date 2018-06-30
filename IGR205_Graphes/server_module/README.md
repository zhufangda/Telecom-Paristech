# Server Module

This part is used to create a server in order to receive the request of page web in visualisation module.

In this part, we use the flask to create a web server.User use the GET method.

The web server support the followig api: 

localhost:port/summary_graph?keyword=[your key word]&k=[your distance]?database=[sembib|dblp]

# How To use
Before use this server, you have to user the programm in the directory <code>
workspace/</code>

- 1. Go to the workspace file and run the following command:
<code> java -classpath algo-windows.jar Indexing data/sembib.nt 200 0</code>
- 2. Copy all the data in the directory <code>worksapce\index</code> to the directory <code>Server\sembib\index</code>
- 3. Copy the <code>sembib.nt</code> in the the  <code>Server\sembib\data</code>

if you want to support the database DBLP, follow these steps:

- 1. Go to the workspace file and run the following command:
<code> java -classpath algo-windows.jar Indexing data/delp.nt 200 0</code>
- 2. Copy all the directory <code>worksapce\index</code> to the directory <code>Server\dblp\index</code>
- 3. Copy the <code>DBLP.nt</code> in the the  <code>Server\sembib\data</code>
When all the data is prepared, you can run following command to run the server:

<code>
python app.py
</code>

Before run this code, you have to ensure that you have installed the python 3.0 and the package <code>flask</code>