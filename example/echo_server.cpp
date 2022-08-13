#include "tcp_server.hpp"
#include "net_state.hpp"

using namespace fannetwork;

int main(int argc, const char* argv[]) {
  auto server = TcpServer::create();
  server->init(atoi(argv[1]));
  server->start();
  return 0;
}