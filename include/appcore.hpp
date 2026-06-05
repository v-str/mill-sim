#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "common.hpp"
#include "tcp_server.hpp"

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

namespace MillSim {

    /// @brief Application entry point.
    /// @details Owns the io_context, signal handling, and TCP server.
class AppCore {
   public:
    /// @brief AppCore constructor.
    /// @param mode  Operational mode (Simulation / Production).
    AppCore(OperationalMode mode);
    ~AppCore() = default;

    /// @brief Run the io_context event loop (blocking).
    void run();

   private:
    /// @brief Wait for SIGINT/SIGTERM and trigger graceful shutdown.
    awaitable<void> waitForShutdown();

    asio::io_context m_context;
    asio::signal_set m_signals;
    TcpServer m_tcpServer;

    OperationalMode m_mode;
};

}  // namespace MillSim
