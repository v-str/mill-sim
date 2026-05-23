#include "mill_tcp_server.hpp"

#include <iostream>

bool mill::TcpConnection::m_isConnectionLocked = false;

mill::TcpConnection::TcpConnection(asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)) {}

void mill::TcpConnection::doRead() {
  auto self = shared_from_this();

  asio::async_read_until(
      m_socket, m_buffer, '\n',
      [this, self](const boost::system::error_code& error_code,
                   size_t /*bytes_transferred*/) {
        if (!error_code) {
          std::istream is(&m_buffer);
          std::string command;
          std::getline(is, command);

          if (!command.empty() && command.back() == '\r') {
            command.pop_back();
          }

          std::cout << "SERVER RECEIVED: " << command << std::endl;

          if (command == "shutdown") {
            std::cout << "terminate" << std::endl;
            m_socket.close();
            std::terminate();
          }

          doRead();
        } else {
          handleError(error_code, "doRead");
        }
      });
}

void mill::TcpConnection::doWrite(const std::string& response) {
  std::string data = "SERVER: " + response + "\n";

  auto self = shared_from_this();

  asio::async_write(m_socket, asio::buffer(data),
                    [this, self](const boost::system::error_code& error_code,
                                 size_t /*bytes_transferred*/) {
                      if (!error_code) {
                        doRead();
                      } else {
                        handleError(error_code, "doWrite");
                      }
                    }

  );
}

void mill::TcpConnection::handleError(
    const boost::system::error_code& error_code, const std::string& text) {
  if (error_code == asio::error::eof ||
      error_code == asio::error::connection_reset) {
    std::cout << "Client disconnected: " << error_code.message() << std::endl;
  } else {
    std::cerr << "Error in " << text << ": " << error_code.message()
              << std::endl;
  }
}

mill::MillTcpServer::MillTcpServer()
    : m_acceptor(m_context,
                 asio::ip::tcp::endpoint(asio::ip::tcp::v4(), mill::PORT)) {}

void mill::MillTcpServer::setupServer() {
  client_listen();
  m_context.run();
}

void mill::MillTcpServer::client_listen() {
  auto socket = std::make_shared<asio::ip::tcp::socket>(m_context);

  std::cout << "Server listening..." << std::endl;

  m_acceptor.async_accept(
      *socket,
      [this, socket](const boost::system::error_code& error_code) {
        if (!error_code) {
          auto connection = std::make_shared<TcpConnection>(std::move(*socket));
          std::cout << "Connection established" << std::endl;
          connection->doRead();
        }

        client_listen();
      }

  );
}

void mill::MillTcpServer::run() { m_context.run(); }
