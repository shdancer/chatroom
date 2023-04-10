#include "socket.hpp"
#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

namespace chatroom {
namespace net {

Socket::Socket(const char *addr, in_port_t port) {
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&this->addr, sizeof(addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = htons(port);
  this->addr.sin_addr.s_addr = inet_addr(addr);
}

int Socket::start() {
  int suc = bind(sock_fd, (sockaddr *)&addr, sizeof(sockaddr));
  listen(sock_fd, 100);
  return suc;
}

int Socket::accept() {
  int len = sizeof(addr);
  return ::accept(sock_fd, (sockaddr *)&addr, (socklen_t *)&len);
}

} // namespace net
} // namespace chatroom
