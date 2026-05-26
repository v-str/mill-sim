#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "common.hpp"
#include "tcp_server.hpp"

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

namespace MillSim {

class AppCore {
   public:
    AppCore(OperationalMode mode);
    ~AppCore() = default;

    void run();

   private:
    awaitable<void> waitForShutdown();

    asio::io_context m_context;
    asio::signal_set m_signals;
    TcpServer m_server;

    OperationalMode m_mode;
};

}  // namespace MillSim
