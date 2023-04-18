#pragma once

#include "crp.hpp"
#include "iostream"
#include "server.hpp"
#include "thread_pool/task.hpp"
#include <pthread.h>
namespace chatroom {
namespace net {

struct ReceiveTaskData {
  CRP *crp_ptr;
  Server *server_ptr;
};

class ReceiveTask : public thread_pool::Task {
public:
  void exec(void *receive_task_data) override {

    chatroom::net::CRP *c = ((ReceiveTaskData *)receive_task_data)->crp_ptr;
    chatroom::net::Server *server =
        ((ReceiveTaskData *)receive_task_data)->server_ptr;
  }

  ReceiveTask(void *args) : thread_pool::Task(args){};
};

} // namespace net
} // namespace chatroom