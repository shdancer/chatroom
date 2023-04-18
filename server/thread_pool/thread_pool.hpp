#pragma once

#include "queue"
#include "task.hpp"
namespace chatroom {
namespace thread_pool {

class Worker;

class ThreadPool {
  friend class Worker;

public:
  ThreadPool(int thread_num);
  void submit(Task *);

private:
  pthread_mutex_t q_mutex;
  pthread_cond_t q_cond;

  std::queue<Task *> task_q;
  Worker *workers;
};

} // namespace thread_pool
} // namespace chatroom