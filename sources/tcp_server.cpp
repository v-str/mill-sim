#include "tcp_server.hpp"

#include <iostream>

MillSim::TcpServer::TcpServer(asio::io_context* pCtx)
    : m_context(pCtx),
      m_acceptor(*m_context,
                 asio::ip::tcp::endpoint(ip::tcp::v4(), MillSim::PORT)) {}
