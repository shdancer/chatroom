#pragma once

#include "crp.hpp"
#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <cstring>
#include <pthread.h>
namespace chatroom {
namespace net {

struct LogoutTaskData {
  CRPMessage *message_ptr;
  Server *server_ptr;
};

class LogoutTask : public thread_pool::Task {
public:
  void exec(void *login_task_data) override {

    chatroom::net::CRPMessage *message =
        ((LogoutTaskData *)login_task_data)->message_ptr;
    chatroom::net::Server *server =
        ((LogoutTaskData *)login_task_data)->server_ptr;
    pthread_rwlock_wrlock(server->get_sender_fd_rwlock());
    if (server->get_sender_fd()->count(message->sender)) {
      server->get_sender_fd()->erase(message->sender);
      std::cout << "logout succeeded" << std::endl;
    }
    pthread_rwlock_unlock(server->get_sender_fd_rwlock());

    delete message;
  }

  LogoutTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom