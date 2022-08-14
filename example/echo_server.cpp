#include "tcp_server.hpp"
#include "net_state.hpp"

using namespace fannetwork;

int main(int argc, const char* argv[]) {
  auto server = std::make_shared<TcpServer>();

  server->init(atoi(argv[1]), 2);
  server->start();

  return 0;
}