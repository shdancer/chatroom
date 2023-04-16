#include "task.hpp"

namespace chatroom {
namespace thread_pool {

Task::Task() { args = nullptr; }

Task::Task(void *args) : args(args) {}

Task::~Task() {}

} // namespace thread_pool
} // namespace chatroom