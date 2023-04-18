#pragma once

#include "crp.hpp"
#include "iostream"
#include "login_task.hpp"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <pthread.h>
namespace chatroom {
namespace net {

struct ReceiveTaskData {
  CRP *crp_ptr;
  Server *server_ptr;
};

class ReceiveTask : public thread_pool::Task {
public:
  void exec(void *receive_task_data) override {

    chatroom::net::CRP *c = ((ReceiveTaskData *)receive_task_data)->crp_ptr;
    chatroom::net::Server *server =
        ((ReceiveTaskData *)receive_task_data)->server_ptr;
    chatroom::net::CRPMessage *message = new CRPMessage;
    int res = c->receive(message);
    if (res == -1) {
      c->close();
      delete server->get_fd_crp()[c->get_fd()];
      delete message;
      std::cout << "closed: " << c->get_fd() << std::endl;
      return;
    }
    if (res == 0) {
      switch (message->op_code) {
      case LOGIN:
        LoginTaskData *data = new LoginTaskData{
            .server_ptr = server, .message_ptr = message, .fd = c->get_fd()};
        LoginTask *task = new LoginTask(data);
        server->get_pool()->submit(task);
        break;
      }
      // message->DEBUG();
      std::cout << "from: " << pthread_self() << std::endl;
    }
    pthread_mutex_lock(server->get_read_set_mutex());
    FD_SET(c->get_fd(), server->get_read_set());
    pthread_mutex_unlock(server->get_read_set_mutex());

    delete message;
  }

  ReceiveTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom