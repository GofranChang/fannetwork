#include "tcp_server.hpp"
#include "net_state.hpp"

using namespace fannetwork;

int main(int argc, const char* argv[]) {
  std::shared_ptr<DefaultAcceptHandler> handler = std::make_shared<DefaultAcceptHandler>();

  auto server = std::make_shared<TcpServer>(handler);
  handler->set_server(server.get());

  server->init(atoi(argv[1]), 2);
  server->start();

  return 0;
}