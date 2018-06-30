//
//  tcpserver: TCP/IP INET Server.
//  (c) Eric Lecolinet - Telecom ParisTech - 2016.
//  http://www.telecom-paristech.fr/~elc
//

#ifndef __tcpserver__
#define __tcpserver__
#include <memory>
#include <pthread.h>
#include "cppsocket.h"

namespace cppu {
  class TCPConnection;
  class TCPLock;
  
  /** @brief TCP/IP IPv4 server.
   *  The server supports TCP/IP AF_INET connections (following the IPv4 Internet protocol)
   *  with multiple clients. One thread is used per client.
   *
   *  Call setCallback() to specify the callback method that will be invoked each
   *  time a request is sent by a client then run() to start the server.
   *
   *  Requests can be processed concurrently thanks to threads. To avoid concurrency
   *  problems the callback can perform a read or write lock (@see TCPLock).
   */
  class TCPServer {
  public:
    /// @brief constructor: initializes the TCPServer.
    TCPServer();
    
    /// @brief destructor: cleans up the TCPServer.
    virtual ~TCPServer();
    
    /** @brief starts the TCPServer.
     *  run() binds an internal ServerSocket to _port_ then starts an infinite loop
     *  that processes connection requests from clients.
     *
     *  For each successful connection request, a TCPConnection object is created.
     *  This object starts a thread that processes incoming requests from its client.
     *  A callback method (@see setCallback()) is invoked for each request.
     *
     *  @return 0 on normal termination or a negative value if the ServerSocket
     *  could not be bound (value is then one of Socket::Errors).
     */
    virtual int run(int port);
    
    /** @brief changes the callback method of the TCPServer.
     *  This callback is called each time the TCPServer receives a request from
     *  a client. It can be any method of _object_ with the following parameters:
     *    - _cnx_ is the connection with the client sending the request
     *    - _request_ contains the data sent by the client
     *    - _response_ will be sent to the client as a response
     *  The connection is closed if the callback returns false.
     *
     *  To avoid concurrency problems, the callback should perform a read or write
     *  lock (@see TCPLock) before performing a computation.
     */
    template <class T>
    void setCallback(T& object,
                     bool (T::*method)(TCPConnection& cnx, const std::string& request, std::string& response))
    {
      _callbackPtr.reset(new CallbackMethod<T>(object, method));
      _callback = _callbackPtr.get();
    }
    
    /// Callback interface.
    struct Callback {
      virtual bool call(TCPConnection& cnx, const std::string& request, std::string& response) = 0;
    };
    
    /** @brief changes the callback object of the TCPServer.
     *  @see setCallback(object, method).
     */
    void setCallback(Callback& callback) {
      _callbackPtr.reset();
      _callback = &callback;
    }

    /// returns the internal ServerSocket.
    ServerSocket& serverSocket() {return _servsock;}
    
    /// prints warning and error messages on the terminal.
    virtual void error(const std::string& msg, const TCPConnection* = nullptr);
    
  private:
    friend class TCPLock;
    friend class TCPConnection;
    TCPServer(const TCPServer&);             // the copy constr is disabled.
    TCPServer& operator=(const TCPServer&);  // assignment is disabled.
    
  protected:
    /// creates a new connection that starts a new thread for listening this socket.
    virtual TCPConnection* createCnx(Socket*);
    
    template <class T>
    struct CallbackMethod : public Callback {
      typedef bool (T::*Fun)(TCPConnection&, const std::string&, std::string&);
      T& obj;
      Fun method;
      CallbackMethod(T& obj, Fun method) : obj(obj), method(method) {}
      virtual bool call(TCPConnection& cnx, const std::string& req, std::string& resp) {
        return (obj.*method)(cnx, req, resp);
      }
    };
    
    // instance variables.
    ServerSocket _servsock;
    std::shared_ptr<Callback> _callbackPtr;
    Callback* _callback;
    pthread_rwlock_t _threadlock;
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /** @brief Locks the server in read mode or in write mode.
   *  Must be created *in the stack* by the callback method.
   */
  class TCPLock {
  public:
    /** @brief locks the server in *write* or *read* mode.
     *  In order to avoid concurrency problems between threads, the callback method
     *  (@see setCallback()) can create a TCPLock object *in the stack* before
     *  performing a computation.
     *
     *  _writeMode_ must be true if the callback changes data and false (the default)
     *  otherwise. A writeMode lock blocks all other locks (and the corresponding
     *  threads) until the callback method returns.
     */
    TCPLock(TCPConnection& cnx, bool writeMode = false);
    ~TCPLock();
  private:
    TCPConnection& _cnx;
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /** @brief Connection with a given client.
   *  Each TCPConnection uses a different thread.
   */
  class TCPConnection : public SocketBuffer {
  public:
    TCPServer& server() {return _server;}
    pthread_t  thread() {return _thread;}
  private:
    friend class TCPServer;
    TCPServer& _server;
    pthread_t  _thread;
    TCPConnection(TCPServer&, Socket*);
    static void* start(void*);
    void processRequests();
  };
}
#endif

