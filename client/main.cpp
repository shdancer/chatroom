#include "crp.hpp"
#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  chatroom::net::SocketStreamClient s("127.0.0.1", 8080);

  int suc = s.connect();
  int fd = s.get_sock_fd();

  chatroom::net::CRP c(fd);

  char const *str = "hello world";
  chatroom::net::CRPMessage message(std::strlen(str) + 1 + 11,
                                    chatroom::net::CHAT, 0, 1, str);
  c.send(&message);
  s.close();
  return 0;
}