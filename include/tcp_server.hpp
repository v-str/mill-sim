#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "common.hpp"

namespace MillSim {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;

class TcpServer {
   public:
    TcpServer(asio::io_context* pCtx);

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    ~TcpServer() = default;

    void stop();

   private:
    void setupServer();
    awaitable<void> listen();
    awaitable<void> echo(ip::tcp::socket socket);

    asio::io_context* m_context;
    ip::tcp::acceptor m_acceptor;
};

}  // namespace MillSim
