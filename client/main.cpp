#include "crp.hpp"
#include "socket.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/_pthread/_pthread_t.h>
#include <unistd.h>

void *quit(void *client_ptr) {
  while (true) {
    char ch = getchar();
    if (ch == 'q') {
      ((chatroom::net::SocketStreamClient *)client_ptr)->close();
      exit(0);
    }
  }
}

int main(int argc, char *argv[]) {
  chatroom::net::SocketStreamClient s("127.0.0.1", 8080);
  int suc = s.connect();
  int fd = s.get_sock_fd();

  pthread_t tid;
  pthread_create(&tid, nullptr, quit, (void *)(&s));

  chatroom::net::CRP c(fd);

  char const *str = "hello world";
  chatroom::net::CRPMessage message(std::strlen(str) + 1 + 11,
                                    chatroom::net::CHAT, atoi(argv[1]), 1, str);
  while (true) {
    c.send(&message);
    sleep(2);
    // std::cout << "send over" << std::endl;
  }
  return 0;
}