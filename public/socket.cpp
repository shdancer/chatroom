#include "socket.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/_types/_socklen_t.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

namespace chatroom {
namespace net {

SocketStream::SocketStream(const char *addr, in_port_t port) {
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&this->addr, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = htons(port);
  this->addr.sin_addr.s_addr = inet_addr(addr);
}

int SocketStream::close() { return ::close(sock_fd); }

SocketStreamHost::SocketStreamHost(const char *addr, in_port_t port)
    : SocketStream(addr, port){};

int SocketStreamHost::host() {
  int suc = bind(sock_fd, (sockaddr *)&this->addr, sizeof(sockaddr));
  if (suc != 0) {
    std::cout << "bind failed: " << errno << std::endl;
    return suc;
  }
  std::cout << "listen on fd: " << sock_fd << std::endl;
  return ::listen(sock_fd, 100);
}

int SocketStreamHost::accept() {
  sockaddr_in client_addr;
  int len = sizeof(client_addr);
  return ::accept(sock_fd, (sockaddr *)&client_addr, (socklen_t *)&len);
}

SocketStreamClient::SocketStreamClient(const char *addr, in_port_t port)
    : SocketStream(addr, port) {}

int SocketStreamClient::connect() {
  int len = sizeof(addr);
  return ::connect(sock_fd, (const sockaddr *)&addr, len);
}

int SocketStream::get_sock_fd() { return sock_fd; }

} // namespace net
} // namespace chatroom
