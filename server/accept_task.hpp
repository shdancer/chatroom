#pragma once

#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <pthread.h>
namespace chatroom {
namespace net {

class AcceptTask : public thread_pool::Task {
public:
  void exec(void *server_ptr) override {
    Server *server = (Server *)server_ptr;
    int fd = server->accept();
    std::cout << "accepted on fd: ";

    pthread_rwlock_wrlock(server->get_rwlock());
    // chatroom::net::CRP *crp = server->get_fd_crp()[fd];
    // if (crp == nullptr) {
    server->get_fd_crp()[fd] = new chatroom::net::CRP(fd);
    // }
    pthread_rwlock_unlock(server->get_rwlock());

    pthread_mutex_lock(server->get_mutex());
    FD_SET(fd, server->get_read_set());
    std::cout << fd << std::endl;
    pthread_mutex_unlock(server->get_mutex());
  }

  AcceptTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom