#include "tcp_server.hpp"

#include <boost/asio/read_until.hpp>
#include <iostream>

namespace MillSim {

TcpServer::~TcpServer() {
  stop();
  if (m_asioThread.joinable()) {
    m_asioThread.join();
  }
}

void TcpServer::runServer(unsigned short port, unsigned short connectionCount) {
  m_connectionCount = connectionCount;
  m_ioContext = std::make_unique<asio::io_context>();
  m_acceptor = std::make_unique<ip::tcp::acceptor>(
      *m_ioContext, ip::tcp::endpoint(ip::tcp::v4(), port));

  setupServer();
  m_asioThread = std::thread([this] { m_ioContext->run(); });
}

void TcpServer::stop() {
  if (m_ioContext) {
    m_ioContext->stop();
  }
  if (m_acceptor) {
    boost::system::error_code ec;
    m_acceptor->close(ec);
  }
}

void TcpServer::setupServer() {
  co_spawn(*m_ioContext, listen(), asio::detached);
}

awaitable<void> TcpServer::listen() {
  try {
    while (true) {
      ip::tcp::socket socket =
          co_await m_acceptor->async_accept(asio::use_awaitable);

      co_spawn(m_acceptor->get_executor(), echo(std::move(socket)), detached);
    }
  } catch (const boost::system::system_error& e) {
    std::cerr << "listen error: " << e.code().message() << std::endl;
  }
}

awaitable<void> TcpServer::echo(ip::tcp::socket socket) {
  try {
    asio::streambuf buf;
    while (true) {
      co_await asio::async_read_until(socket, buf, '\n', asio::use_awaitable);

      std::istream is(&buf);
      std::string line;
      std::getline(is, line);

      if (m_onMessage) {
        m_onMessage(line);
      }

      std::string response = "Echo: " + line + '\n';
      co_await async_write(socket, asio::buffer(response), asio::use_awaitable);
    }
  } catch (const boost::system::system_error& e) {
    std::cerr << "echo error: " << e.code().message() << std::endl;
  }
}

void TcpServer::setOnMessage(std::function<void(std::string)> callback) {
  m_onMessage = callback;
}

}  // namespace MillSim
