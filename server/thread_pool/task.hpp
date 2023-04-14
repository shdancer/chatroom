#pragma once

namespace chatroom {
namespace thread_pool {

class Worker;

class Task {
  friend class Worker;

public:
  Task();
  Task(void *);

  virtual void exec(void *) = 0;

protected:
  void *args;
};

} // namespace thread_pool
} // namespace chatroom