#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/socket.h>

int main() {
  chatroom::net::SocketStreamClient s("0.0.0.0", 18080, "127.0.0.1", 8080);

  int suc = s.connect();
  int fd = s.get_sock_fd();
  char str[] = "123";
  char buf[1024];
  send(fd, str, strlen(str), 0);
  recv(fd, buf, strlen(str), 0);
  std::cout << buf << std::endl;
  return 0;
}