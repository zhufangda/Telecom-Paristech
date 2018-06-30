//
//  tcpserver: TCP/IP INET Server.
//  (c) Eric Lecolinet - Telecom ParisTech - 2016.
//  http://www.telecom-paristech.fr/~elc
//

#include <unistd.h>
#include <iostream>
#include "tcpserver.h"
using namespace std;

namespace cppu {
  
  TCPConnection::TCPConnection(TCPServer& server, Socket* socket)
  : SocketBuffer(socket), _server(server), _thread(0) {
    // pthread_create() creates a thread which calls start()
    ::pthread_create(&_thread, NULL, start, this);
  }
  
  void* TCPConnection::start(void* cnxptr) {  // called by pthread_create()
    TCPConnection* cnx = static_cast<TCPConnection*>(cnxptr);
    // start the loop processing incoming requests
    cnx->processRequests();
    
    // liberer les ressources
    delete cnx->_sock;    // detruit donc ferme le socket
    pthread_t thread = cnx->_thread;
    delete cnx;
    if (thread != 0) pthread_exit(NULL);    // ATTENTION: doit etre en dernier !!!
    return nullptr;
  }
  
  // starts an infinite loop that processes incoming requests on a TCPServer::Cnx connection.
  void TCPConnection::processRequests() {
    while (true) {
      string request, response;
      
      // read the incoming request sent by the client
      // SocketBuffer::readLine() lit jusqu'au premier délimiteur (qui est supprimé)
      ssize_t received = readLine(request);
      
      if (received < 0) {
        _server.error("Read error", this);
        return;
      }
      if (received == 0) {
        _server.error("Connection closed by client", this);
        return;
      }
      
      // process the request
      if (!_server._callback) {
        response = "OK";
      }
      // ferme la connection si la valeur de retour est false
      else if (!_server._callback->call(*this, request, response)) {
        _server.error("Closing connection with client", this);
        return;
      }
      
      // toujours envoyer une reponse au client sinon il va se bloquer !
      // SocketBuffer::writeLine() envoie tout et rajoute le delimiteur
      ssize_t sent = writeLine(response);
      
      if (sent < 0) {
        _server.error("Write error", this);
        return;
      }

      if (sent == 0) {
        _server.error("Connection closed by client", this);
        return;
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TCPLock::TCPLock(TCPConnection& cnx, bool writeMode) : _cnx(cnx) {
    if (writeMode)
      pthread_rwlock_wrlock(&(_cnx.server()._threadlock)); // blocks in WRITE mode
    else
      pthread_rwlock_rdlock(&(_cnx.server()._threadlock)); // blocks in READ mode
  }
  
  TCPLock::~TCPLock() {
    pthread_rwlock_unlock(&(_cnx.server()._threadlock));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TCPServer::TCPServer() : _servsock() {
    pthread_rwlock_init(&_threadlock, NULL);
  }
  
  TCPServer::~TCPServer() {}
  
  TCPConnection* TCPServer::createCnx(Socket* sock) {
    return new TCPConnection(*this, sock);
  }
    
  int TCPServer::run(int port) {
    int status = _servsock.bind(port);  // lier le ServerSocket a ce port
    
    if (status < 0) {
      error("Can't bind on port: " + to_string(port));
      return status;   // returns negative value, see Socket::bind()
    }
    
    while (true) {
      Socket* socket = _servsock.accept();
      if (!socket) {
        error("accept() failed");
        continue;     // cas d'erreur : va a la prochaine iteration de while()
      }
      
      // lance la lecture des messages de ce socket dans un thread
      TCPConnection* c = createCnx(socket);
      if (c->thread() == 0) {
        error("pthread_create() failed");
        delete c;
      }
    }
    return 0;  // means OK
  }
  
  void TCPServer::error(const string& msg, const TCPConnection* cnx) {
    if (cnx)
      cerr << "TCPServer: " << msg << " (connection: " << cnx << ")" << endl;
    else
      cerr << "TCPServer: " << msg << endl;
  }
}
