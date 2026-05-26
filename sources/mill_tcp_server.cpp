#include "mill_tcp_server.hpp"

#include <iostream>

mill::MillTcpServer::MillTcpServer()
    : m_acceptor(m_context,
                 asio::ip::tcp::endpoint(asio::ip::tcp::v4(), mill::PORT)) {
    listen();
}

void mill::MillTcpServer::start() {
    std::cout << "Mill TCP Server started" << std::endl;

    m_context.run();
}

void mill::MillTcpServer::listen() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_context);
}

void mill::MillTcpServer::acceptHandler(const boost::system::error_code& ec,
                                        asio::ip::tcp::socket peer) {}
