#include "crp.hpp"
#include "server.hpp"
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
namespace chatroom {
namespace net {

class Server;

struct PrintTaskData {
  CRP *crp_ptr;
  Server *server_ptr;
};

class PrintTask : public thread_pool::Task {
public:
  void exec(void *print_task_data) override {

    chatroom::net::CRP *c = ((PrintTaskData *)print_task_data)->crp_ptr;
    chatroom::net::Server *server =
        ((PrintTaskData *)print_task_data)->server_ptr;
    chatroom::net::CRPMessage message;
    int res = c->receive(&message);
    if (res == -1) {
      c->close();
      delete server->get_fd_crp()[c->get_fd()];
      std::cout << "closed: " << c->get_fd() << std::endl;
      return;
    }
    if (res == 0) {
      message.DEBUG();
      std::cout << "from: " << pthread_self() << std::endl;
    }
    pthread_mutex_lock(server->get_mutex());
    FD_SET(c->get_fd(), server->get_read_set());
    pthread_mutex_unlock(server->get_mutex());
  }

  PrintTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom