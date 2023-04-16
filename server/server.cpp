#include "server.hpp"
#include "accept_task.hpp"
#include "crp.hpp"
#include "print_task.hpp"
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
  pthread_rwlock_init(&record_rwlock, nullptr);
  pthread_mutex_init(&read_set_mutex, nullptr);
  memset(fd_crp, 0, sizeof(fd_crp));
  FD_ZERO(&read_set);

  tcp_server.host();
  int fd = tcp_server.get_sock_fd();
  FD_SET(fd, &read_set);
}

void Server::run() {
  // std::cout << "running" << std::endl;
  while (true) {
    pthread_mutex_lock(&read_set_mutex);
    fd_set readable_set = read_set;
    pthread_mutex_unlock(&read_set_mutex);
    // std::cout << (int)FD_ISSET(3, &readable_set) << std::endl;
    timeval zero = {.tv_sec = 0, .tv_usec = 0};
    select(1024, &readable_set, nullptr, nullptr, &zero);
    // std::cout << "listen: "
    //           << (int)FD_ISSET(tcp_server.get_sock_fd(), &readable_set)
    //           << std::endl;
    if (FD_ISSET(tcp_server.get_sock_fd(), &readable_set)) {
      net::AcceptTask *task = new AcceptTask(this);
      pool.submit(task);
    }
    // std::cout << FD_ISSET(4, &read_set) << std::endl;
    for (int fd = 3; fd < 1024; fd++) {
      if (FD_ISSET(fd, &readable_set) == 0) {
        continue;
      }
      if (fd == tcp_server.get_sock_fd()) {
        continue;
      }
      std::cout << "recv fd: " << fd << std::endl;

      pthread_rwlock_rdlock(&record_rwlock);
      CRP *crp = fd_crp[fd];
      pthread_rwlock_unlock(&record_rwlock);

      pthread_mutex_lock(&read_set_mutex);
      FD_CLR(fd, &read_set);
      pthread_mutex_unlock(&read_set_mutex);

      PrintTaskData *data =
          new PrintTaskData{.server_ptr = this, .crp_ptr = crp};

      PrintTask *task = new PrintTask(data);
      pool.submit(task);
    }
  }
}

int Server::accept() { return tcp_server.accept(); }

pthread_rwlock_t *Server::get_rwlock() { return &record_rwlock; }
pthread_mutex_t *Server::get_mutex() { return &read_set_mutex; }
fd_set *Server::get_read_set() { return &read_set; }
CRP **Server::get_fd_crp() { return fd_crp; }

} // namespace net
} // namespace chatroom