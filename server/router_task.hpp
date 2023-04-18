#pragma once

#include "crp.hpp"
#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <pthread.h>
namespace chatroom {
namespace net {

struct RouterTaskData {
  CRPMessage *message_ptr;
  Server *server_ptr;
  int fd;
};

class ReceiveTask : public thread_pool::Task {
public:
  void exec(void *router_task_data) override {

    chatroom::net::CRPMessage *message =
        ((RouterTaskData *)router_task_data)->message_ptr;
    chatroom::net::Server *server =
        ((RouterTaskData *)router_task_data)->server_ptr;
    int fd = ((RouterTaskData *)router_task_data)->fd;

    pthread_rwlock_rdlock(server->get_sender_fd_rwlock());
    int exist = server->get_sender_fd()->count(fd);
    pthread_rwlock_unlock(server->get_sender_fd_rwlock());

    if (exist) {
      pthread_rwlock_rdlock(server->get_sender_fd_rwlock());
      int fd = server->get_sender_fd()->at(message->receiver);
      pthread_rwlock_unlock(server->get_sender_fd_rwlock());

      pthread_mutex_lock(&server->get_message_queue_mutex()[fd]);
      server->get_message_queue()[fd].push(message);
      pthread_mutex_unlock(&server->get_message_queue_mutex()[fd]);
    } else {
    }
  }

  ReceiveTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom