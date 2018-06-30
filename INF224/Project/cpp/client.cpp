//
// Client C++ pour communiquer avec un serveur TCP
// eric lecolinet - telecom paristech - 2016
//

#include <iostream>
#include <string>
#include <algorithm>
#include "cppsocket.h"
using namespace std;
using namespace cppu;

static const string HOST = "localhost";
static const int PORT = 3331;

///
/// Lit une requete depuis le Terminal, envoie cette requete au serveur,
/// recupere sa reponse et l'affiche sur le Terminal.
/// Noter que le programme bloque si le serveur ne repond pas.
///

int main() {
  Socket sock;
  SocketBuffer sockbuf(sock);
  
  int status = sock.connect(HOST, PORT);
  
  if (status < 0) {
    switch (status) {
      case Socket::Failed:
        cerr << "Client: Couldn't reach host "<<HOST<<":"<<PORT << endl;
        return 1;
      case Socket::UnknownHost:
        cerr << "Client: Couldn't find host "<<HOST<<":"<<PORT << endl;
        return 1;
      default:
        cerr << "Client: Couldn't connect host "<<HOST<<":"<<PORT << endl;
        return 1;
    }
  }
  
  cout << "Client connected to "<< HOST<<":"<<PORT << endl;
  
  while (cin) {
    cout << "Request: ";
    string request, response;
    
    getline(cin, request);
    if (request == "quit") return 0;
    
    // Envoyer la requete au serveur
    if (sockbuf.writeLine(request) < 0) {
      cerr << "Client: Couldn't send message" << endl;
      return 2;
    }
    
    // Recuperer le resultat envoye par le serveur
    if (sockbuf.readLine(response) < 0) {
      cerr << "Client: Couldn't receive message" << endl;
      return 2;
    }
    
    // Le serveur remplace les '\n' par des ';' car '\n' sert a indiquer la
    // fin d'un message entre le client et le serveur
    // On fait ici la transformation inverse
    replace(response.begin(), response.end(), ';', '\n');
    
    cout << "Response: " << response << endl;
  }
}
