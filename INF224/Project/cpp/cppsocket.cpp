//
//  socket: Classes for TCP/IP and UDP Datagram INET Sockets.
//  (c) Eric Lecolinet - Telecom ParisTech - 2015.
//  http://www.telecom-paristech.fr/~elc
//
//  Classes:
//  - Socket: TCP/IP or UDP Datagram Socket.
//  - ServerSocket: TCP/IP Socket Server.
//  - SocketBuffer: utility class for preserving record boundaries when exchanging
//    data blocks or text lines between TCP/IP sockets.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>      // fcntl.h  won't compile without unistd.h !
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <csignal>
#include "cppsocket.h"
using namespace std;

namespace cppu {
  
  Socket::Socket(int type) :
  // - family is AF_INET (other families such as AF_UNIX or AF_INET6 are not supported)
  // - type can be SOCK_STREAM (TCP/IP) or SOCK_DGRAM (datagram connection)
  // - protocol is 0 (meaning it is chosen automatically)
  _sockfd(::socket(/*family*/AF_INET, type, /*protocol*/0))
  {
    // ignore SIGPIPES when possible
#if defined(SO_NOSIGPIPE)
    int set = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
#endif
  }
  
  Socket::Socket(int type, int sockfd) : _sockfd(sockfd) {
  }
  
  Socket::~Socket() {
    close();  // closes the socket.
  }
  
  // for INET4 sockets
  int Socket::setLocalAddress(struct sockaddr_in& addr, int port) {
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return 0;
  }
  
  // for INET4 sockets
  int Socket::setAddress(struct sockaddr_in& addr, const string& host, int port) {
    struct hostent* hostinfo = NULL;
    // gethostbyname() is obsolete!
    if (host.empty() || !(hostinfo = ::gethostbyname(host.c_str()))) return -1; // host not found
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // creer l'adresse du remote host a partir de son nom
    ::memcpy((void *)&addr.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
    return 0;
  }
  
  int Socket::bind(int port) {
    if (_sockfd < 0) return InvalidSocket;
    // for INET4 sockets
    struct sockaddr_in addr;
    setLocalAddress(addr, port);
    // assigns the address specified by addr to sockfd (returns -1 on error, 0 otherwise)
    return ::bind(_sockfd, (const struct sockaddr*)&addr, sizeof(addr));
  }
  
  int Socket::bind(const string& host, int port) {
    if (_sockfd < 0) return InvalidSocket;
    // for INET4 sockets
    struct sockaddr_in addr;
    if (setAddress(addr, host, port) < 0) return UnknownHost;
    // assigns the address specified by addr to sockfd (returns -1 on error, 0 otherwise)
    return ::bind(_sockfd, (const struct sockaddr*)&addr, sizeof(addr));
  }
  
  int Socket::connect(const string& host, int port) {
    if (_sockfd < 0) return InvalidSocket;
    // for INET4 sockets
    struct sockaddr_in addr;
    if (setAddress(addr, host, port) < 0) return UnknownHost;
    // connects sockfd to the address specified by addr (returns -1 on error, 0 otherwise)
    return ::connect(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
  }
  
  int Socket::close() {
    int stat = 0;
    if (_sockfd >= 0) {
      stat = ::shutdown(_sockfd, SHUT_RDWR);  // SHUT_RDWR=2
      stat += ::close(_sockfd);
    }
    _sockfd = -1;
    return stat;
  }
  
  void Socket::shutdownInput() {
    ::shutdown(_sockfd, 0);
  }
  
  void Socket::shutdownOutput() {
    ::shutdown(_sockfd, 1/*SD_SEND*/);
  }
  
  int Socket::setReceiveBufferSize(int size) {
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int));
  }
  
  int Socket::setSendBufferSize(int size) {
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(int));
  }
  
  int Socket::setReuseAddress(bool state) {
    int set = state;
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
  }
  
  int Socket::setSoLinger(bool on, int time) {
    struct linger l;
    l.l_onoff = on;          // Linger active
    l.l_linger = time;       // How long to linger for
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
  }
  
  int Socket::setSoTimeout(int timeout) {
    struct timeval tv;
    tv.tv_sec = timeout / 1000;             // ms to seconds
    tv.tv_usec = (timeout % 1000) * 1000;   // ms to microseconds
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  }
  
  int Socket::setTcpNoDelay(bool state) {
    int set = state;
    return ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, &set, sizeof(int));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ServerSocket::ServerSocket()    // See Socket's notes.
  : _sockfd(::socket(AF_INET, SOCK_STREAM, 0)) {}
  
  ServerSocket::~ServerSocket() {
    close();
  }
  
  Socket* ServerSocket::createSocket(int sockfd) {
    return new Socket(SOCK_STREAM, sockfd);
  }
  
  int ServerSocket::bind(int port, int backlog) {
    if (_sockfd < 0) return Socket::InvalidSocket;
    // ne marche que pour IN4 !
    struct sockaddr_in addr = {0}; //memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (::bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) return -1;
    // verifications sur le serveur
    socklen_t taille = sizeof addr;
    if (::getsockname(_sockfd, (struct sockaddr*)&addr, &taille) < 0) return -1;
    
    // le serveur se met en attente sur le socket d'ecoute
    // listen s'applique seulement aux sockets de type SOCK_STREAM ou SOCK_SEQPACKET.
    if (::listen(_sockfd, backlog) < 0) return -1;
    // port = ntohs(addr.sin_port);
    return 0;
  }
  
  int ServerSocket::close() {
    int stat = 0;
    if (_sockfd >= 0) {
      //::shutdown(sockfd, SHUT_RDWR);
      ::close(_sockfd);
    }
    _sockfd = -1;  // indiquer inutilisable par write/read()
    return stat;
  }
  
  Socket* ServerSocket::accept() {
    int sock_com = -1;
    // cf. man -s 3n accept, attention EINTR ou EWOULBLOCK ne sont pas geres!
    if ((sock_com = ::accept(_sockfd, NULL, NULL)) < 0) return nullptr;
    else return createSocket(sock_com);
  }
  
  int ServerSocket::setReceiveBufferSize(int size) {
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int));
  }
  
  int ServerSocket::setReuseAddress(bool state) {
    int set = state;
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
  }
  
  int ServerSocket::setSoTimeout(int timeout) {
    struct timeval tv;
    tv.tv_sec = timeout / 1000;             // ms to seconds
    tv.tv_usec = (timeout % 1000) * 1000;   // ms to microseconds
    return ::setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  }
  
  int ServerSocket::setTcpNoDelay(bool state) {
    // turn off TCP coalescence for INET sockets (useful in some cases to avoid delays)
    int set = state;
    return ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&set, sizeof(int));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  struct InputBuffer {
    InputBuffer(size_t size) : buffer(new char[size]), begin(buffer), end(buffer+size), remaining(0) {
    }
    
    ~InputBuffer() {
      delete[] buffer;
    }
    char* buffer;
    char* begin;
    char* end;
    ssize_t remaining;
  };
  
  SocketBuffer::SocketBuffer(Socket* sock, size_t inSize, size_t outSize) :
  _inSize(inSize),
  _outSize(outSize),
  _inSep(-1),  // means '\r' or '\n' or "\r\n"
  _outSep('\n'),
  _sock(sock),
  _in(nullptr) {
  }
  
  SocketBuffer::SocketBuffer(Socket& sock, size_t inSize, size_t outSize)
  : SocketBuffer(&sock, inSize, outSize) {
  }
  
  SocketBuffer::~SocketBuffer() {
    delete _in;
  }
  
  void SocketBuffer::setInputSeparator(int separ) {
    _inSep = separ;
  }
  
  void SocketBuffer::setOutputSeparator(int separ) {
    _outSep = separ;
  }
  
  ssize_t SocketBuffer::readLine(string& str) {
    str.clear();
    if (!_sock) return Socket::InvalidSocket;
    if (!_in) _in = new InputBuffer(_inSize);
    
    while (true) {
      if (retrieveLine(str, _in->remaining)) return str.length()+1;
      // - received > 0: data received
      // - received = 0: nothing received (shutdown or empty message)
      // - received < 0: an error occurred
      ssize_t received = _sock->receive(_in->begin, _in->end-_in->begin);
      //cout << "received: "<< received << " " << _in->begin << endl;
      if (received <= 0) return received;     // -1 (error) or 0 (shutdown)
      if (retrieveLine(str, received)) return str.length()+1;
    }
  }
  
  
  bool SocketBuffer::retrieveLine(string& str, ssize_t received) {
    if (received <= 0 || _in->begin > _in->end) {
      _in->begin = _in->buffer;
      return false;
    }
    
    // search for separator
    char* sep = nullptr;
    int sepLen = 1;
    
    if (_inSep < 0) {     // means: '\r' or '\n' or "\r\n"
      for (char* p = _in->begin; p < _in->begin+received; ++p) {
        if (*p == '\n') {
          sep = p;
          sepLen = 1;
          break;
        }
        else if (*p == '\r') {
          sep = p;
          if (p < _in->begin+received-1 && *(p+1) == '\n') {
            sepLen = 2;
          }
          break;
        }
      }
    }
    else {
      for (char* p = _in->begin; p < _in->begin+received; ++p)
        if (*p == _inSep) {
          sep = p;
          break;
        }
    }
    
    if (sep) {
      str.append(_in->begin, sep-_in->begin);
      _in->remaining = received - (sep+sepLen - _in->begin);
      _in->begin = sep+sepLen;
      return true;
    }
    else {
      str.append(_in->begin, received);
      _in->begin = _in->buffer;
      _in->remaining = 0;
      return false;
    }
  }
  
  
  ssize_t SocketBuffer::writeLine(const string& str) {
    if (!_sock) return Socket::InvalidSocket;
    
    // a negature value of _outSep means that \r\n must be added
    size_t len = str.length() + (_outSep < 0 ? 2 : 1);
    
    // if len is not too large, try to send everything in one block
    if (len <= _outSize) {
      char buffer[len];
      ::memcpy(buffer, str.c_str(), str.length());   // copy string
      if (_outSep >= 0) buffer[len-1] = _outSep;     // add separator
      else {
        buffer[len-2] = '\r';
        buffer[len-1] = '\n';
      }
      return write(buffer, len);
    }
    else {
      ssize_t sent = write(str.c_str(),str.length());
      if (_outSep >= 0)
        sent += _sock->send(&_outSep, 1);
      else
        sent+= _sock->send("\r\n", 2);
      return sent;
    }
  }
  
  
  ssize_t SocketBuffer::write(const char* s, size_t len) {
    if (!_sock) return Socket::InvalidSocket;
    const char* begin = s;
    const char* end = s + len;
    ssize_t totalSent = 0;
    
    while (begin < end) {
      // - sent > 0: data sent
      // - sent = 0: file was shutdown
      // - sent < 0: an error occurred
      ssize_t sent = _sock->send(begin, end-begin);
      if (sent <= 0) return sent;     // -1 (error) or 0 (shutdown)
      begin += sent;
      totalSent += sent;
    }
    return totalSent;
  }
  
  
  ssize_t SocketBuffer::read(char* s, size_t len) {
    if (!_sock) return Socket::InvalidSocket;
    char* begin = s;
    char* end = s + len;
    ssize_t totalReceived = 0;
    
    while (begin < end) {
      // - received > 0: data received
      // - received = 0: nothing received (shutdown or empty message)
      // - received < 0: an error occurred
      ssize_t received = _sock->receive(begin, end-begin);
      if (received <= 0) return received;     // -1 (error) or 0 (shutdown)
      begin += received;
      totalReceived += received;
    }
    return totalReceived;
  }
  
}

