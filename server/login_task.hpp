#pragma once

#include "crp.hpp"
#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <cstring>
#include <pthread.h>
#include <sys/_pthread/_pthread_mutex_t.h>
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

      CRPMessage *msg = new CRPMessage(14 + 11, LOGIN, 0, 0, "login success");
      pthread_mutex_lock(&server->get_message_queue_mutex()[fd]);
      server->get_message_queue()[fd]->push(msg);
      pthread_mutex_unlock(&server->get_message_queue_mutex()[fd]);

      pthread_mutex_lock(server->get_write_set_mutex());
      FD_SET(fd, server->get_write_set());
      pthread_mutex_unlock(server->get_write_set_mutex());
      std::cout << "login success" << std::endl;
    } else {
      CRPMessage *msg = new CRPMessage(13 + 11, LOGIN, 0, 0, "login failed");
      pthread_mutex_lock(&server->get_message_queue_mutex()[fd]);
      server->get_message_queue()[fd]->push(msg);
      pthread_mutex_unlock(&server->get_message_queue_mutex()[fd]);

      pthread_mutex_lock(server->get_write_set_mutex());
      FD_SET(fd, server->get_write_set());
      pthread_mutex_unlock(server->get_write_set_mutex());

      std::cout << "login failed" << std::endl;
    }

    delete message;
  }

  LoginTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom