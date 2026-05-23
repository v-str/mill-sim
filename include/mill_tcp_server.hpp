#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "common.hpp"

namespace mill {

namespace asio = boost::asio;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(asio::ip::tcp::socket socket);

  void doRead();
  void doWrite(const std::string& response);

 private:
  void handleError(const boost::system::error_code& error_code,
                   const std::string& text);

  asio::ip::tcp::socket m_socket;
  asio::streambuf m_buffer;

  static bool m_isConnectionLocked;
};

class MillTcpServer {
 public:
  MillTcpServer();

  MillTcpServer(const MillTcpServer&) = delete;
  MillTcpServer& operator=(const MillTcpServer&) = delete;

  ~MillTcpServer() = default;

  void setupServer();
  void run();

 private:
  void client_listen();

  asio::io_context m_context;
  asio::ip::tcp::acceptor m_acceptor;
};

}  // namespace mill
