#include "crp.hpp"
#include "thread_pool/task.hpp"
#include <iostream>
#include <pthread.h>
namespace chatroom {
namespace net {

class PrintTask : public thread_pool::Task {
public:
  void exec(void *fd_ptr) override {
    int fd = *(int *)fd_ptr;
    chatroom::net::CRP c(fd);
    chatroom::net::CRPMessage message;
    while (true) {
      int res = c.receive(&message);
      if (res == -1) {
        break;
      }
      if (res == 0) {
        message.DEBUG();
        std::cout << "from: " << pthread_self() << std::endl;
      }
    }
  }

  PrintTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom