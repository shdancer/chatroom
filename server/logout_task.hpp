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
  int fd;
};

class LogoutTask : public thread_pool::Task {
public:
  void exec(void *logout_task_data) override {

    chatroom::net::CRPMessage *message =
        ((LogoutTaskData *)logout_task_data)->message_ptr;
    chatroom::net::Server *server =
        ((LogoutTaskData *)logout_task_data)->server_ptr;
    int fd = ((LogoutTaskData *)logout_task_data)->fd;

    pthread_rwlock_rdlock(server->get_sender_fd_rwlock());
    int exist = server->get_sender_fd()->count(message->sender);
    pthread_rwlock_unlock(server->get_sender_fd_rwlock());

    if (exist) {
      pthread_rwlock_wrlock(server->get_sender_fd_rwlock());
      server->get_sender_fd()->erase(fd);
      pthread_rwlock_unlock(server->get_sender_fd_rwlock());

      CRPMessage *msg = new CRPMessage(14 + 11, LOGIN, 0, 0, "logout success");
      pthread_mutex_lock(&server->get_message_queue_mutex()[fd]);
      server->get_message_queue()[fd].push(msg);
      pthread_mutex_unlock(&server->get_message_queue_mutex()[fd]);

      pthread_mutex_lock(server->get_write_set_mutex());
      FD_SET(fd, server->get_write_set());
      pthread_mutex_unlock(server->get_write_set_mutex());
      std::cout << "logout success" << std::endl;
    } else {
      CRPMessage *msg = new CRPMessage(13 + 11, LOGIN, 0, 0, "no login");
      pthread_mutex_lock(&server->get_message_queue_mutex()[fd]);
      server->get_message_queue()[fd].push(msg);
      pthread_mutex_unlock(&server->get_message_queue_mutex()[fd]);

      pthread_mutex_lock(server->get_write_set_mutex());
      FD_SET(fd, server->get_write_set());
      pthread_mutex_unlock(server->get_write_set_mutex());

      std::cout << "no login" << std::endl;
    }

    delete message;
  }

  LogoutTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom