#pragma once

#include <memory>
#include "net_state.hpp"

class ThreadPool;

class EventHandler;

namespace fannetwork {

using EventCb = std::function<void(int, short)>;

using IOEventCb = std::function<void(int, std::string)>;

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

  NetState regist_accept_socket(int fd, std::shared_ptr<EventHandler> handler);

  void run();

private:


private:
  std::shared_ptr<Reactor> main_reactor_;

  std::shared_ptr<Reactor> connection_reactor_;

  std::shared_ptr<Reactor> task_reactor_;

  std::unique_ptr<ThreadPool> thread_pool_;
};

}