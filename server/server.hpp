#pragma once

#include "crp.hpp"
#include "map"
#include "socket.hpp"
#include "thread_pool/thread_pool.hpp"
namespace chatroom {
namespace net {

const std::map<int, const char *> users{{1, "123"}, {2, "234"}};

class Server {
public:
  Server(const char *addr, int port, int thread_num);
  void run();

  int accept();
  pthread_rwlock_t *get_fd_crp_rwlock();
  pthread_rwlock_t *get_sender_fd_rwlock();
  pthread_mutex_t *get_read_set_mutex();
  fd_set *get_read_set();
  CRP **get_fd_crp();
  thread_pool::ThreadPool *get_pool();
  std::map<int, int> *get_sender_fd();
  void receive(CRP *c);

private:
  chatroom::net::CRP *fd_crp[1024];
  std::map<int, int> sender_fd;
  chatroom::thread_pool::ThreadPool pool;

  SocketStreamHost tcp_server;
  fd_set read_set;
  fd_set write_set;

  pthread_rwlock_t fd_crp_rwlock;
  pthread_rwlock_t sender_fd_rwlock;
  pthread_mutex_t read_set_mutex;
  pthread_mutex_t write_set_mutex;

  int thread_num;
  int max_fd;
};
} // namespace net
} // namespace chatroom
