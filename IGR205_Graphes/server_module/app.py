from flask import Flask, json
from flask import request
from flask_cors import *
import os


import os
import networkx as nx

app = Flask(__name__)

CORS(app, supports_credentials=True)

def summary_graph(keyword, k = 20, database = 'sembib', verbose=False):
    parent_path = os.path.realpath(os.path.dirname(os.path.abspath(__file__)))
    jar_path = parent_path + "\\algo-windows.jar"

    if database == "sembib":
        os.chdir(parent_path + "\sembib")
        print("Current dir：",os.getcwd())
        command = "java -classpath " + jar_path + " SummaryGraph data/sembib.nt data/keyword.txt " + str(k)
    else:
        os.chdir(parent_path + "\dblp")
        print("Current dir：",os.getcwd())
        command = "java -classpath " + jar_path + " SummaryGraph data/DBLP.nt data/keyword.txt " + str(k)

    with open("data/keyword.txt",'w' ) as file:
        file.write(keyword)
        
    print("Eecute command:" + command)
    output = os.popen(command).read()
    if verbose:
        a = str.split("\n")
        for b in a:
            print(b)

    return transform_to_json()


@app.route('/')
def hello_world():
    print('Hello World-2!')
    return 'Hello World-24!'


@app.route('/summary_graph', methods=['POST', 'GET'])
def get_summary_graph():
    error = None
    print(request.method)
    if request.method == 'POST':
        print(request.form['keyword'])
    # the code below is executed if the request method
    # was GET or the credentials were invalid
    if request.method == 'GET':
        keyword = request.args['keyword']
        k = request.args['k']
        database = request.args['database']
        print("Get request:","keyword:",keyword,"\tdistance:", k, "\tdatabase:", database)
    reponse = summary_graph(keyword, k, database, verbose=True)
    print("Get reponse")
    return reponse

if __name__ == '__main__':
    app.run(debug=True)




def back_summay_fin(keyword, k=20, database='sembib', verbose=False):
        with open("data/keyword.txt", 'w') as file:
            file.write(keyword)

        command = "java -classpath ../workspace/algo-windows.jar SummaryGraph data/sembib.nt data/keyword.txt " + str(k)
        print("Eecute command:" + command)
        output = os.popen(command).read()
        if verbose:
            a = str.split("\n")
            for b in a:
                print(b)
        return transform_to_json()

def transform_to_json():
    print("Traitment of graph...")
    edges = []
    with open('index/summary.txt', 'r', encoding='utf-8') as file:
        spamreader = file.readlines()
        for row in spamreader:
            row = row.replace('> <', '>\t<')
            row = row.replace('" <', '"\t<')
            row = row.replace('> "', '>\t"')
            edges.append(row.split('\t'))

    G = nx.MultiDiGraph()
    for edge in edges:
        G.add_edge(edge[0], edge[2], label=edge[1], id=edge[1])

   # bb = nx.betweenness_centrality(G)
    for node in G.nodes():
        G.nodes[node]['degree'] = G.degree(node)
        G.nodes[node]['in_degree'] = G.in_degree(node)
        G.nodes[node]['out_degree'] = G.out_degree(node)
      #  G.nodes[node]['betweenness_centrality'] = bb[node]
    return json.dumps(nx.node_link_data(G))









