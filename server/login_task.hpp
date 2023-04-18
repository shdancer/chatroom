#pragma once

#include "crp.hpp"
#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <cstring>
#include <pthread.h>
namespace chatroom {
namespace net {

struct LoginTaskData {
  CRPMessage *message_ptr;
  Server *server_ptr;
  int fd;
};

class LoginTask : public thread_pool::Task {
public:
  void exec(void *login_task_data) override {

    chatroom::net::CRPMessage *message =
        ((LoginTaskData *)login_task_data)->message_ptr;
    chatroom::net::Server *server =
        ((LoginTaskData *)login_task_data)->server_ptr;
    int fd = ((LoginTaskData *)login_task_data)->fd;

    if (strcmp(users.at(message->sender), (char *)message->data) == 0) {
      pthread_rwlock_wrlock(server->get_sender_fd_rwlock());
      server->get_sender_fd()->insert({message->sender, fd});
      pthread_rwlock_unlock(server->get_sender_fd_rwlock());
    }
    std::cout << "login failed" << std::endl;

    delete message;
  }

  LoginTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom