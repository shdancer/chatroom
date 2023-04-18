#include "server.hpp"
#include <pthread.h>

chatroom::net::CRP *fd_sender[1024];
pthread_rwlock_t record_mutex;

int main() {
  chatroom::net::Server server("0.0.0.0", 8080, 4);

  server.run();
  return 0;
}