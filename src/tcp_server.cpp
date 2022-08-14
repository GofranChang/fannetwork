#include "tcp_server.hpp"

#include "common/logger.h"
#include "net_state.hpp"
#include "socket.h"
#include "task_scheduler.h"
#include "event_handler.h"

namespace fannetwork {

class TcpServerDefaultHandler : public EventHandler {
public:
  virtual void on_event(int fd, short event) override {}

  virtual void on_ioevent(int fd, const char* data, size_t len) override {}

  virtual void on_error(int fd, short event) override {}
};

class TcpServerImpl : public TcpServer {
public:
  TcpServerImpl(const std::shared_ptr<EventHandler>& handler);

  virtual NetState init(int16_t port, int32_t thread_num) override;

  virtual void start() override;

private:
  std::shared_ptr<Socket> accept_socket_;

  std::shared_ptr<EventHandler> handler_;
};

TcpServerImpl::TcpServerImpl(const std::shared_ptr<EventHandler>& handler) :
    handler_(handler) {
}

NetState TcpServerImpl::init(int16_t port, int32_t thread_num) {
  INITLOGGER(spdlog::level::trace, "", true);

  auto scheduler = TaskScheduler::instance();
  scheduler->init(thread_num);

  if (accept_socket_) {
    GLOGE("Init tcp server failed, cur accpet socket exist, fd : {}", accept_socket_->fd());
    return NetState::MULTI_OPERATOR;
  }

  accept_socket_ = Socket::create(Socket::SocketType::TCP_SOCKET);
  if (!accept_socket_) {
    GLOGE("Create accept socket failed, reason {}", strerror(errno));
    return NetState::IO_OPERATOR_ERR;
  }

  if (accept_socket_->open() != NetState::SUCCESS) {
    GLOGE("TCP server err : create socket failed, reason {}", strerror(errno));
    return NetState::INTERNAL_ERR;
  }

  if (accept_socket_->set_non_block() != NetState::SUCCESS) {
    GLOGE("TCP server err : set socket non block failed");
    return NetState::INTERNAL_ERR;
  }

  if (accept_socket_->bind("0.0.0.0", port) != NetState::SUCCESS) {
    GLOGE("TCP server err : bind local addr failed");
    return NetState::INTERNAL_ERR;
  }

  if (accept_socket_->listen(10) != NetState::SUCCESS) {
    GLOGE("TCP server err : bind local addr failed");
    return NetState::INTERNAL_ERR;
  }

  GLOGD("TCP : server socket init success");

  return NetState::SUCCESS;
}

void TcpServerImpl::start() {
  auto scheduler = TaskScheduler::instance();
  scheduler->run();
}

std::shared_ptr<TcpServer> TcpServer::create(const std::shared_ptr<EventHandler> & handler) {
  auto internal_handler = handler;
  if (!internal_handler) {
    internal_handler = std::make_shared<TcpServerDefaultHandler>();
  }
  auto p = new TcpServerImpl(internal_handler);
  return std::shared_ptr<TcpServer>(p);
}

}