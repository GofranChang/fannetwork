#pragma once

#include <memory>
#include "net_state.hpp"

class ThreadPool;

namespace fannetwork {

class Reactor;

class TaskScheduler {
public:
  static TaskScheduler* instance() {
    static TaskScheduler* p = new TaskScheduler();
    return p;
  }

private:
  TaskScheduler();

  TaskScheduler(TaskScheduler&) = delete;

  TaskScheduler(TaskScheduler&&) = delete;

  TaskScheduler operator()(TaskScheduler&) = delete;

  TaskScheduler operator()(TaskScheduler&&) = delete;

public:
  ~TaskScheduler() = default;

public:
  NetState init(size_t thread_nums);

  void run();

private:


private:
  std::shared_ptr<Reactor> accept_reactor_;

  std::shared_ptr<Reactor> connection_reactor_;

  std::shared_ptr<Reactor> task_reactor_;

  std::unique_ptr<ThreadPool> thread_pool_;
};

}