#pragma once
#include "crp.hpp"
#include "server.hpp"
#include "thread_pool/task.hpp"

namespace chatroom {
namespace net {

struct ReceiveTaskData {
  CRP *crp_ptr;
  Server *server_ptr;
};

class SendTask : public thread_pool::Task {
public:
  void exec(void *send_task_data) override {}

  SendTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom