#include "socket.hpp"
#include <arpa/inet.h>
#include <iostream>
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
  int suc = bind(sock_fd, (sockaddr *)&this->addr, sizeof(sockaddr));
  if (suc != 0) {
    std::cerr << "bind failed: " << errno << std::endl;
  }
}

int SocketStream::close() { return ::close(sock_fd); }

SocketStreamHost::SocketStreamHost(const char *addr, in_port_t port)
    : SocketStream(addr, port){};

int SocketStreamHost::listen() { return ::listen(sock_fd, 100); }

int SocketStreamHost::accept() {
  int len = sizeof(addr);
  return ::accept(sock_fd, (sockaddr *)&addr, (socklen_t *)&len);
}

SocketStreamClient::SocketStreamClient(const char *addr, in_port_t port,
                                       const char *tar_addr, in_addr_t tar_port)
    : SocketStream(addr, port) {
  // std::cout << "hello" << std::endl;
  bzero(&this->tar_addr, sizeof(this->tar_addr));
  this->tar_addr.sin_family = AF_INET;
  this->tar_addr.sin_port = htons(tar_port);
  this->tar_addr.sin_addr.s_addr = inet_addr(tar_addr);
}

int SocketStreamClient::connect() {
  int len = sizeof(tar_addr);
  return ::connect(sock_fd, (const sockaddr *)&tar_addr, len);
}

int SocketStreamClient::get_sock_fd() { return sock_fd; }

} // namespace net
} // namespace chatroom
