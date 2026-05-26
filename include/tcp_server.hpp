#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "common.hpp"

namespace MillSim {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

class TcpServer {
   public:
    TcpServer(asio::io_context* pCtx);

    ~TcpServer() = default;

   private:
    asio::io_context* m_context;
    ip::tcp::acceptor m_acceptor;
};

}  // namespace MillSim
