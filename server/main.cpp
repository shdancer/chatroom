#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>
int main() {
  chatroom::net::Socket s("0.0.0.0", 8080);
  s.start();

  while (true) {
    int fd = s.accept();
    char s[1024];
    int size = recv(fd, s, 1024, 0);
    send(fd, s, strlen(s), 0);
    close(fd);
  }
  return 0;
}