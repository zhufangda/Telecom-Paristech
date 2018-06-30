//
//  cppsocket: Classes for TCP/IP and UDP Datagram INET Sockets.
//  (c) Eric Lecolinet - Telecom ParisTech - 2016.
//  http://www.telecom-paristech.fr/~elc
//
//  Classes:
//  - Socket: TCP/IP or UDP/Datagram socket (for AF_INET connections following IPv4).
//  - ServerSocket: TCP/IP Socket Server.
//  - SocketBuffer: preserves record boundaries when exchanging data between TCP/IP sockets.
//

#ifndef __cppsocket__
#define __cppsocket__
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

namespace cppu {
  
  // ignore SIGPIPES when possible
#if defined(MSG_NOSIGNAL)
#  define _NO_SIGPIPE(flags) (flags | MSG_NOSIGNAL)
#else
#  define _NO_SIGPIPE(flags) (flags)
#endif
  
  /** @brief TCP/IP or UDP/Datagram socket.
   * This class encapsulates a TCP/IP or UDP/Datagram socket.
   * AF_INET connections following the IPv4 Internet protocol are supported.
   *
   * @note ServerSocket should be used on the server side (@see ServerSocket).
   * @note SIGPIPE signals are ignored when using Linux, BSD or MACOSX.
   * @note TCP/IP sockets do not preserve record boundaries, @see SocketBuffer for a solution.
   */
  class Socket {
  public:
    /** @brief Socket errors.
     * - Socket::Failed (-1): connection error (could not connect, could not bind, etc.)
     * - Socket::InvalidSocket (-2): invalid socket or wrong socket type
     * - Socket::UnknownHost (-3): could not reach host
     */
    enum Errors {Failed = -1, InvalidSocket = -2, UnknownHost = -3};
    
    /** @brief Creates a new Socket.
     * Creates a AF_INET socket using the IPv4 Internet protocol.
     * Type can be:
     * - SOCK_STREAM (the default) for TCP/IP connected stream sockets
     * - SOCK_DGRAM for UDP/datagram sockets
     */
    Socket(int type = SOCK_STREAM);
    
    /// Creates a Socket object from an existing socket file descriptor.
    Socket(int type, int sockfd);
    
    /// Destructor (closes the socket).
    virtual ~Socket();
    
    /** @brief Assigns the socket to the local address.
     * Typically used for UDP/Datagram sockets, @see Unix bind() system call for details.
     * @return 0 on success or a negative value on error which is one of Socket::Errors
     */
    virtual int bind(int port);
    
    /** @brief Assigns the socket to an address.
     * Typically used for UDP/Datagram sockets, @see Unix bind() system call for details.
     * @return 0 on success or a negative value on error which is one of Socket::Errors
     */
    virtual int bind(const std::string& host, int port);
    
    /** @brief Connects the socket to an address.
     * Typically used for TCP/IP sockets on the client side, @see Unix connect() system call
     * for details and ServerSocket for TCP/IP sockets on the server side.
     * @return 0 on success or a negative value on error which is one of Socket::Errors
     */
    virtual int connect(const std::string& host, int port);
    
    /** @brief Closes the socket.
     * @return 0 on success and -1 on error.
     */
    virtual int close();
    
    /// Returns true if the socket has been closed.
    bool isClosed() const {return _sockfd < 0;}
    
    /// Returns the Unix descriptor of the socket.
    int descriptor() {return _sockfd;}
    
    /** @brief Sends data to a connected socket.
     * Sends _len_ bytes to a TCP/IP socket using the Unix send() function (@see recv() for details)
     *
     * @return the number of bytes that were sent or:
     * - _len_ is 0 or shutdownInput() was called on the other side,
     * - Socket::Failed (-1): a connection error occured.
     *
     * @note that TCP/IP sockets do not preserve record boundaries, @see SocketBuffer for a solution.
     */
    ssize_t send(const void* buf, size_t len, int flags = 0) {
      return ::send(_sockfd, buf, len, _NO_SIGPIPE(flags));
    }
    
    /** @brief Receives data from a connected socket.
     * Reads at most _len_ bytes from a TCP/IP socket using the Unix recv() function.
     * By default, this function blocks the caller until data is present (@see recv() for details).
     *
     * @return the number of bytes that were received or:
     * - 0: _len_ is 0 or shutdownOutput() was called on the other side,
     * - Socket::Failed (-1): a connection error occured.
     *
     * @note that TCP/IP sockets do not preserve record boundaries, @see SocketBuffer for a solution.
     */
    ssize_t receive(void* buf, size_t len, int flags = 0) {
      return ::recv(_sockfd, buf, len, flags);
    }
    
    /** @brief Sends data to a datagram socket.
     * Sends _len_ bytes to a datagram socket using the Unix sendto() function.
     * @return the number of bytes that were sent or Socket::Failed (-1) if an error occurred.
     */
    ssize_t sendTo(const void* buf, size_t len, int flags,
                   const struct sockaddr* dest_addr, socklen_t addrlen) {
      return ::sendto(_sockfd, buf, len, _NO_SIGPIPE(flags), dest_addr, addrlen);
    }
    
    /** @brief Receives data from datagram socket.
     * Reads at most _len_ bytes from a datagram socket using the Unix recvfrom() function.
     * By default, this function blocks the caller until data is present (@see recvfrom() for details).
     * @return the number of bytes which was received or Socket::Failed (-1) if an error occurred.
     */
    ssize_t receiveFrom(void* buf, size_t len, int flags,
                        struct sockaddr* src_addr, socklen_t* addrlen) {
      return ::recvfrom(_sockfd, buf, len, flags, src_addr, addrlen);
    }
    
    /// Disables further receive operations.
    virtual void shutdownInput();
    
    /// Disables further send operations.
    virtual void shutdownOutput();
    
    /// Sets the size of the TCP/IP input buffer.
    int setReceiveBufferSize(int size);
    
    /// Enables/disables the SO_REUSEADDR socket option.
    int setReuseAddress(bool);
    
    /// Sets the size of the TCP/IP output buffer.
    int setSendBufferSize(int size);
    
    /// Enables/disables SO_LINGER with the specified linger time in seconds.
    int setSoLinger(bool, int linger);
    
    /// Enables/disables SO_TIMEOUT with the specified timeout (in milliseconds).
    int setSoTimeout(int timeout);
    
    /// Enables/disables TCP_NODELAY (turns on/off TCP coalescence).
    int setTcpNoDelay(bool);
    
    /// Gets the size of the TCP/IP input buffer.
    int getReceiveBufferSize() const;
    
    /// Gets SO_REUSEADDR state.
    bool getReuseAddress() const;
    
    /// Gets the size of the TCP/IP output buffer.
    int getSendBufferSize() const;
    
    /// Gets SO_LINGER state and the specified linger time in seconds.
    bool getSoLinger(int& linger) const;
    
    /// Gets SO_TIMEOUT value.
    int getSoTimeout() const;
    
    /// Gets TCP_NODELAY state.
    bool getTcpNoDelay() const;
    
    /// Initializes a local INET4 address, returns 0 on success, -1 otherwise.
    virtual int setLocalAddress(struct sockaddr_in& addr, int port);
    
    /// Initializes a remote INET4 address, returns 0 on success, -1 otherwise.
    virtual int setAddress(struct sockaddr_in& addr, const std::string& host, int port);
    
  private:
    friend class ServerSocket;
    int _sockfd;
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&&) = delete;
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /** @brief TCP/IP server socket.
   * This class implements a TCP/IP socket that waits for requests to come in over the network.
   * AF_INET connections following the IPv4 Internet protocol are supported.
   * @note TCP/IP sockets do not preserve record boundaries, @see SocketBuffer for a solution.
   */
  class ServerSocket {
  public:
    /** @brief Creates a new server socket.
     * Creates a listening socket that waits for connection requests by TCP/IP clients.
     */
    ServerSocket();
    
    virtual ~ServerSocket();
    
    /** @brief Accepts a new connection request and returns the corresponding socket.
     * By default, this function blocks the caller until a connection is present.
     * @return the new Socket or nullptr on error.
     */
    virtual Socket* accept();
    
    /** @brief Assigns the socket to the local address.
     * The socket must be bound before using it.
     * @return 0 on success or a negative value on error which is one of Socket::Errors
     */
    virtual int bind(int port, int backlog = 50);
    
    /// Closes the socket.
    virtual int close();
    
    /// Returns true if the socket has been closed.
    bool isClosed() const {return _sockfd < 0;}
    
    /// Returns the Unix descriptor of the socket.
    int descriptor() {return _sockfd;}
    
    /// Sets the SO_RCVBUF option to the specified value.
    int setReceiveBufferSize(int size);
    
    /// Enables/disables the SO_REUSEADDR socket option.
    int setReuseAddress(bool);
    
    /// Enables/disables SO_TIMEOUT with the specified timeout (in milliseconds).
    int	setSoTimeout(int timeout);
    
    /// Turns on/off TCP coalescence (useful in some cases to avoid delays).
    int setTcpNoDelay(bool);
    
  protected:
    virtual Socket* createSocket(int sockfd);
    
  private:
    int _sockfd;  // listening socket.
    ServerSocket(const ServerSocket&) = delete;
    ServerSocket& operator=(const ServerSocket&) = delete;
    ServerSocket& operator=(ServerSocket&&) = delete;
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /** @brief Preserves record boundaries when exchanging data between connected TCP/IP sockets.
   * This class ensures that one call to writeLine() corresponds to one and exactly one call
   * to readLine() on the other side.
   * This differs from the behavior of Socket::send() and Socket::receive() because TCP/IP
   * connected sockets do not preserve record boundaries. writeLine() and readLine() solve
   * this problem by automatically adding and searching for a separator between successive lines
   * @see setInputSeparator() and setOutputSeparator().
   */
  class SocketBuffer {
  public:
    /** @brief constructor.
     * _socket_ must be a connected TCP/IP Socket (i.e. of SOCK_STREAM type) that must *not* be
     * deleted while the SocketBuffer is used.
     * _inputBufferSize_ and _ouputBufferSize_ are the sizes of the buffers that used internally
     * for exchanging the data.
     */
    SocketBuffer(Socket* socket, size_t inputBufferSize = 8192, size_t ouputBufferSize = 8192);
    SocketBuffer(Socket& socket, size_t inputBufferSize = 8192, size_t ouputBufferSize = 8192);
    
    virtual ~SocketBuffer();
    
    /// returns the associated socket.
    Socket* socket() {return _sock;}
    
    /// returns the input separator.
    int inputSeparator() const {return _inSep;}
    
    /// returns the output separator.
    int outputSeparator() const {return _outSep;}
    
    /** @brief changes the input separator.
     * This function specifies the character(s) used by readLine() to separate successive lines:
     * - if _separ_ >= 0, readLine() searches for _separ_ to separate lines,
     * - if _separ_ < 0, readLine() searches for \n, \r or \n\r.
     * By default, readLine() for \n, \r or \n\r.
     * @note If the input separator is changed, the output separator must be changed accordingly
     * on the other side of the socket.
     * @see setOutputSeparator().
     */
    virtual void setInputSeparator(int separ);
    
    /** @brief changes the output separator.
     * This function specifies the character(s) used by writeLine() to separate successive lines:
     * - if _separ_ >= 0, writeLine() inserts _separ_ between successive lines,
     * - if _separ_ < 0, writeLine() inserts \n\r between successive lines.
     * By default, writeLine() inserts \n.
     * @note If the output separator is changed, the input separator must be changed accordingly
     * on the other side of the socket.
     * @see setInputSeparator().
     */
    virtual void setOutputSeparator(int separ);
    
    /** @brief Reads a line of text from a connected socket.
     * readLine() receives one line of text sent by writeLine() on the other side.
     * The text is stored in _str_. This method blocks until the complete text line is received.
     *
     * readLine() relies on a separator (by default, \n, \n or \n\r, @see setInputSeparator().
     * This separator is automatically removed (it is not stored in _str_).
     *
     * @return the number of bytes that were received or:
     * - 0: shutdownOutput() was called on the other side
     * - Socket::Failed (-1): a connection error occured
     * - Socket::InvalidSocket (-2): the socket is invalid.
     * The separator is counted in the value returned by readLine().
     */
    virtual ssize_t readLine(std::string& str);
    
    /** @brief Sends a line of text to a connected socket.
     * writeLine() sends one line of text that will be received by a single call to readLine()
     * on the other side (see note below).
     *
     * writeLine() relies on a separator (\n by default, @see setOutputSeparator()) that is automatically
     * inserted between successive lines ().
     *
     * @return the number of bytes that were sent or:
     * - 0: shutdownInput() was called on the other side
     * - Socket::Failed (-1): a connection error occured
     * - Socket::InvalidSocket (-2): the socket is invalid.
     * The separator is counted in the value returned by writeLine().
     *
     * @note if _str_ constains occurences of the separator, readLine() will be called several times on the other side.
     */
    virtual ssize_t writeLine(const std::string& str);
    
    /* Receives a given number of characters from a connected socket.
     * Reads *exactly* _len_ bytes from the socket, blocks otherwise.
     *
     * @return the number of bytes that were received or:
     * - 0: shutdownOutput() was called on the other side
     * - Socket::Failed (-1): a connection error occured
     * - Socket::InvalidSocket (-2): the socket is invalid.
     */
    virtual ssize_t read(char* buffer, size_t len);
    
    /* @brief Sends a given number of characters to a connected socket.
     * Writes _len_ bytes to the socket.
     *
     * @return the number of bytes that were sent or:
     * - 0: shutdownInput() was called on the other side
     * - Socket::Failed (-1): a connection error occured
     * - Socket::InvalidSocket (-2): the socket is invalid.
     */
    virtual ssize_t write(const char* str, size_t len);
    
  private:
    SocketBuffer(const SocketBuffer&) = delete;
    SocketBuffer& operator=(const SocketBuffer&) = delete;
    SocketBuffer& operator=(SocketBuffer&&) = delete;
    
  protected:
    virtual bool retrieveLine(std::string& str, ssize_t received);
    size_t _inSize, _outSize;
    int _inSep, _outSep;
    Socket* _sock;
    struct InputBuffer* _in;
  };
  
}

#endif
