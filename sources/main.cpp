#include "mill_tcp_server.hpp"

int main() {
  /*

  MillMachine machine(opmode::emulation);
  machine.run();

  */

  mill::MillTcpServer tcpServer;
  tcpServer.start();
}
