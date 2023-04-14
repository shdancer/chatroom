#include "crp.hpp"
#include "print_task.hpp"
#include "socket.hpp"
#include "thread_pool/thread_pool.hpp"
#include <iostream>
#include <unistd.h>
int main() {
  chatroom::net::SocketStreamHost s("0.0.0.0", 8080);
  chatroom::thread_pool::ThreadPool pool(4);
  s.host();

  while (true) {
    int *fd = new int;
    *fd = s.accept();
    std::cout << "accepted" << std::endl;

    chatroom::net::PrintTask *p_task = new chatroom::net::PrintTask((void *)fd);
    pool.submit(p_task);
  }
  return 0;
}