#pragma once

#include <stdio.h>

namespace fannetwork {

class EventHandler {
public:
  virtual void on_event(int fd, short event) = 0;

  virtual void on_ioevent(int fd, const char* data, size_t len) = 0;

  virtual void on_error(int fd, short event) = 0;
};

}