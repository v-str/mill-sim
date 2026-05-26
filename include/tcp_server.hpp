#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "common.hpp"

namespace MillSim {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;

using std::cout;
using std::endl;

class TcpServer {
   public:
    TcpServer(asio::io_context* pCtx);

    ~TcpServer() = default;

   private:
    void setupServer();
    awaitable<void> listen();
    awaitable<void> echo(ip::tcp::socket socket);

    asio::io_context* m_context;
    ip::tcp::acceptor m_acceptor;
};

}  // namespace MillSim
