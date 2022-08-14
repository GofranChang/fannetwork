#include "reactor.h"

#include <map>
#include <queue>
#include <thread>

#include "common/logger.h"
#include "event_handler.hpp"

#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"

namespace fannetwork {

class ReactorImpl : public Reactor {
public:
  friend class Reactor;

private:
  ReactorImpl() : run_tid_(std::this_thread::get_id()),
                  evt_base_(event_base_new()),
                  wakeup_event_(nullptr) {
    if (pipe(wakeup_pipe_) != 0) {
      GLOGE("Create reactor wake up pipe failed");
    } else {
      wakeup_event_ = event_new(evt_base_,
                                wakeup_pipe_[0],
                                EV_READ | EV_PERSIST,
                                wakeup_cb,
                                this);

      event_add(wakeup_event_, nullptr);
    }
  }

public:
  virtual ~ReactorImpl() {
    while (!evts_.empty()) {
      auto evt = evts_.front();
      event_free(evt);
      evts_.pop();
    }

    if (nullptr != evt_base_)
      event_base_free(evt_base_);

    evt_base_ = nullptr;
  }

public:
  virtual void register_event(int fd, const std::shared_ptr<EventHandler>& handler) override {
    if (nullptr == evt_base_) {
      GLOGE("Register event error, event base is null, maybe internal error");
      return;
    }

    event* evt = event_new(evt_base_, fd, EV_TIMEOUT | EV_READ | EV_PERSIST, event_cb, handler.get());
    event_add(evt, nullptr);
    evts_.push(evt);
  }

  virtual void register_ioevent(int fd, const std::shared_ptr<EventHandler>& handler) override {
  }

  virtual void register_timer_event() override {
  }

  virtual void start() override {
    if (!evt_base_) {
      GLOGE("Run reactor failed, event base is null");
      return;
    }

    event_base_dispatch(evt_base_);
  }

  virtual void wake_up() override {
    ::write(wakeup_pipe_[1], "1", 1);
  }

  virtual bool is_in_loop() const override { return run_tid_ == std::this_thread::get_id(); }

  static void event_cb(evutil_socket_t fd, short event, void* argc) {
    EventHandler* handler = static_cast<EventHandler*>(argc);
    if (handler) {
      handler->on_event(fd, event);
    }
  }

  static void wakeup_cb(evutil_socket_t fd, short event, void* argc) {
    GLOGE("wakeup_cb");
  }

private:
  std::thread::id run_tid_;

  event_base* evt_base_;

  std::queue<event*> evts_;

  std::map<int, EventCb*> evt_cbs_;

  int wakeup_pipe_[2];

  event* wakeup_event_;
};

std::shared_ptr<Reactor> Reactor::create() {
  Reactor* p = new ReactorImpl;
  return std::shared_ptr<Reactor>(p);
}

Reactor::Reactor() {}

Reactor::~Reactor() {}

}