#pragma once

#include "crp.hpp"
#include "socket.hpp"
#include "thread_pool/thread_pool.hpp"
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_pthread/_pthread_rwlock_t.h>
#include <sys/_types/_fd_def.h>
namespace chatroom {
namespace net {
class Server {
public:
  Server(const char *addr, int port, int thread_num);
  void run();

  int accept();
  pthread_rwlock_t *get_rwlock();
  pthread_mutex_t *get_mutex();
  fd_set *get_read_set();
  CRP **get_fd_crp();

private:
  chatroom::net::CRP *fd_crp[1024];
  chatroom::thread_pool::ThreadPool pool;

  SocketStreamHost tcp_server;
  fd_set read_set;

  pthread_rwlock_t record_rwlock;
  pthread_mutex_t read_set_mutex;
  int thread_num;
};
} // namespace net
} // namespace chatroom
