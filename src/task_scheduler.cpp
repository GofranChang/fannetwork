#include "task_scheduler.h"

#include "common/logger.h"
#include "common/thread_pool.hpp"
#include "reactor.h"
#include "event_handler.hpp"

namespace fannetwork {

TaskScheduler::TaskScheduler() : main_reactor_(nullptr),
                                 connection_reactor_(nullptr),
                                 task_reactor_(nullptr),
                                 thread_pool_(nullptr) {
}

NetState TaskScheduler::init(size_t thread_nums) {
  if (main_reactor_ || connection_reactor_ || task_reactor_ || thread_pool_) {
    GLOGE("Init task scheduler failed, already initialized");
    return NetState::MULTI_OPERATOR;
  }

  GLOGD("Init thread size {}", thread_nums + 2);
  thread_pool_.reset(new ThreadPool(thread_nums + 2));

  main_reactor_ = Reactor::create();

  auto start_reactor = [](std::shared_ptr<Reactor>& reactor) {
    if (reactor) {
      GLOGE("Create reactor failed, already created");
      return;
    }
    GLOGD("Create reactor");
    reactor = Reactor::create();
    reactor->start();
  };

  thread_pool_->enqueue(start_reactor, connection_reactor_);
  thread_pool_->enqueue(start_reactor, task_reactor_);

  return NetState::SUCCESS;
}

NetState TaskScheduler::regist_accept_socket(int fd, std::shared_ptr<EventHandler> handler) {
  if (!main_reactor_ ) {
    GLOGE("Run task scheduler loop failed, main reactor is null");
    return NetState::UNINITIALIZED;
  }

  return NetState::UNINITIALIZED;
}

void TaskScheduler::run() {
  if (!main_reactor_ ) {
    GLOGE("Run task scheduler loop failed, main reactor is null");
    return;
  }

  main_reactor_->start();
}

}