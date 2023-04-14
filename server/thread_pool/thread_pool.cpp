#include "thread_pool.hpp"
#include "thread_pool/worker.hpp"
#include <pthread.h>
#include <sys/signal.h>

namespace chatroom {
namespace thread_pool {

// ThreadPool::
ThreadPool::ThreadPool(int thread_num) {
  Worker::tp = this;
  workers = new Worker[thread_num];
  pthread_mutex_init(&q_mutex, nullptr);
  pthread_cond_init(&q_cond, nullptr);

  for (int i = 0; i < thread_num; i++) {
    workers[i].start();
  }
}

void ThreadPool::submit(Task *task) {
  pthread_mutex_lock(&q_mutex);
  task_q.push(task);
  pthread_cond_signal(&q_cond);
  pthread_mutex_unlock(&q_mutex);
}

} // namespace thread_pool
} // namespace chatroom