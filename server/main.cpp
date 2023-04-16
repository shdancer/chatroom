#include "crp.hpp"
#include "print_task.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "thread_pool/thread_pool.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_pthread/_pthread_rwlock_t.h>
#include <unistd.h>

chatroom::net::CRP *fd_sender[1024];
pthread_rwlock_t record_mutex;

int main() {
  chatroom::net::Server server("0.0.0.0", 8080, 4);

  server.run();
  return 0;
}