#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "common.hpp"

namespace mill {

namespace asio = boost::asio;

class MillTcpServer {
   public:
    MillTcpServer();

    MillTcpServer(const MillTcpServer&) = delete;
    MillTcpServer& operator=(const MillTcpServer&) = delete;

    ~MillTcpServer() = default;

    void start();

   private:
    void listen();
    void acceptHandler(const boost::system::error_code& ec,
                       asio::ip::tcp::socket peer);

    asio::io_context m_context;
    asio::ip::tcp::acceptor m_acceptor;
};

}  // namespace mill
