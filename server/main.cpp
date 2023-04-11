#include "crp.hpp"
#include "socket.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>
int main() {
  chatroom::net::SocketStreamHost s("0.0.0.0", 8080);
  s.host();

  while (true) {
    int fd = s.accept();
    chatroom::net::CRP c(fd);
    chatroom::net::CRPMessage *message = nullptr;
    while (true) {
      message = c.receive();
      if (message != nullptr) {
        break;
      }
    }
    message->DEBUG();

    close(fd);
  }
  return 0;
}