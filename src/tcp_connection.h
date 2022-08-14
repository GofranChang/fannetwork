#pragma once

#include "net_state.hpp"

#include <memory>

namespace fannetwork {

class Socket;

class TcpConnection {
public:
  static std::shared_ptr<TcpConnection> create();

  static std::shared_ptr<TcpConnection> create(const std::shared_ptr<Socket>& socket);

private:
  TcpConnection();

  explicit TcpConnection(const std::shared_ptr<Socket>& socket);

public:
  ~TcpConnection();

public:
  NetState set_conneted_socket(const std::shared_ptr<Socket>& socket);

  NetState read(std::string& content);

  NetState write(const std::string& content);

  void disconnect();

  inline bool is_connected() const { return connecting_; }

private:
  std::shared_ptr<Socket> socket_;

  bool connecting_;
};

}