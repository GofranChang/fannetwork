#pragma once

#include <memory>
#include <functional>

namespace fannetwork {

using EventCb = std::function<void(int, short)>;

using IOEventCb = std::function<void(int, std::string)>;

class ReactorImpl;

class Reactor {
public:
  friend class ReactorImpl;

  static std::shared_ptr<Reactor> create();

private:
  Reactor();

public:
  virtual ~Reactor();

public:
  virtual void register_event(int fd, EventCb* cb) = 0;

  virtual void register_ioevent(int fd,
                                IOEventCb* read_cb,
                                IOEventCb* write_cv,
                                EventCb* error_cb) = 0;

  virtual void register_timer_event() = 0;

  virtual void start() = 0;

  virtual void wake_up() = 0;

  virtual bool is_in_loop() const = 0;
};

}