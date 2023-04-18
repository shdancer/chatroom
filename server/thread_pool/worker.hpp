#pragma once

#include "thread_pool.hpp"
#include <pthread.h>
namespace chatroom {
namespace thread_pool {

class Worker {
  friend class ThreadPool;

public:
  void start();

  static void *work(void *) {
    while (true) {
      pthread_mutex_lock(&tp->q_mutex);
      while (tp->task_q.empty()) {
        pthread_cond_wait(&tp->q_cond, &tp->q_mutex);
      }
      Task *task = tp->task_q.front();
      tp->task_q.pop();
      pthread_mutex_unlock(&tp->q_mutex);

      task->exec(task->args);
      delete task;
    }
    return nullptr;
  }

private:
  pthread_t tid;
  static ThreadPool *tp;
};

} // namespace thread_pool
} // namespace chatroom