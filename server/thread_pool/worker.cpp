#include "worker.hpp"
#include "thread_pool.hpp"
#include <cstdio>
#include <pthread.h>
#include <sys/_pthread/_pthread_t.h>

namespace chatroom {
namespace thread_pool {

ThreadPool *Worker::tp;

void Worker::start() {
  int suc = pthread_create(&tid, nullptr, work, nullptr);
  if (suc != 0) {
    perror("creating thread: ");
  }

  return;
}

} // namespace thread_pool
} // namespace chatroom