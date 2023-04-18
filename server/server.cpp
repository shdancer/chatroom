#include "server.hpp"
#include "accept_task.hpp"
#include "crp.hpp"
#include "thread_pool/thread_pool.hpp"
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_timeval.h>

namespace chatroom {
namespace net {

Server::Server(const char *addr, int port, int thread_num)
    : tcp_server(addr, port), thread_num(thread_num), pool(thread_num) {
  pthread_rwlock_init(&fd_crp_rwlock, nullptr);
  pthread_rwlock_init(&sender_fd_rwlock, nullptr);
  pthread_mutex_init(&read_set_mutex, nullptr);
  pthread_mutex_init(&write_set_mutex, nullptr);

  memset(fd_crp, 0, sizeof(fd_crp));

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);

  tcp_server.host();
  int fd = tcp_server.get_sock_fd();
  max_fd = fd;
  // TODO: add max_fd
  FD_SET(fd, &read_set);
}

void Server::receive(CRP *c) {
  chatroom::net::CRPMessage *message = new CRPMessage;
  int res = c->receive(message);
  if (res == -1) {
    c->close();

    pthread_mutex_lock(&read_set_mutex);
    FD_CLR(c->get_fd(), &read_set);
    pthread_mutex_unlock(&read_set_mutex);

    delete fd_crp[c->get_fd()];
    delete message;
    std::cout << "closed: " << c->get_fd() << std::endl;
    return;
  }
  if (res == 0) {
    switch (message->op_code) {
    case LOGIN:
      // LoginTaskData *data = new LoginTaskData{
      //     .server_ptr = server, .message_ptr = message, .fd = c->get_fd()};
      // LoginTask *task = new LoginTask(data);
      // server->get_pool()->submit(task);
      break;
    }
    message->DEBUG();
  }

  delete message;
}

void Server::run() {
  // std::cout << "running" << std::endl;
  while (true) {
    pthread_mutex_lock(&read_set_mutex);
    fd_set readable_set = read_set;
    pthread_mutex_unlock(&read_set_mutex);

    pthread_mutex_lock(&write_set_mutex);
    fd_set writable_set = write_set;
    pthread_mutex_unlock(&write_set_mutex);

    timeval zero = {.tv_sec = 0, .tv_usec = 0};
    select(1024, &readable_set, &writable_set, nullptr, &zero);

    if (FD_ISSET(tcp_server.get_sock_fd(), &readable_set)) {
      net::AcceptTask *task = new AcceptTask(this);
      pool.submit(task);
    }

    for (int fd = 3; fd < 1024; fd++) {
      if (FD_ISSET(fd, &readable_set) == 0) {
        continue;
      }
      if (fd == tcp_server.get_sock_fd()) {
        continue;
      }
      std::cout << "recv fd: " << fd << std::endl;

      pthread_rwlock_rdlock(&fd_crp_rwlock);
      CRP *crp = fd_crp[fd];
      pthread_rwlock_unlock(&fd_crp_rwlock);

      receive(crp);
    }

    for (int fd = 3; fd < 1024; fd++) {
      if (FD_ISSET(fd, &writable_set) == 0) {
        continue;
      }
      if (fd == tcp_server.get_sock_fd()) {
        continue;
      }
      std::cout << "send fd: " << fd << std::endl;

      pthread_rwlock_rdlock(&fd_crp_rwlock);
      CRP *crp = fd_crp[fd];
      pthread_rwlock_unlock(&fd_crp_rwlock);

      receive(crp);
    }
  }
}

int Server::accept() { return tcp_server.accept(); }

pthread_rwlock_t *Server::get_fd_crp_rwlock() { return &fd_crp_rwlock; }
pthread_rwlock_t *Server::get_sender_fd_rwlock() { return &sender_fd_rwlock; }
pthread_mutex_t *Server::get_read_set_mutex() { return &read_set_mutex; }
fd_set *Server::get_read_set() { return &read_set; }
CRP **Server::get_fd_crp() { return fd_crp; }
thread_pool::ThreadPool *Server::get_pool() { return &pool; }
std::map<int, int> *Server::get_sender_fd() { return &sender_fd; }

} // namespace net
} // namespace chatroom