#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "common.hpp"
#include "tcp_server.hpp"

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

namespace MillSim {

/// @brief Точка входа в приложение.
/// @details Владеет io_context, обработкой сигналов и TCP-сервером.
class AppCore {
   public:
    /// @brief Конструктор ядра приложения.
    /// @param mode  Режим работы (Simulation / Production).
    AppCore(OperationalMode mode);
    ~AppCore() = default;

    /// @brief Запустить цикл обработки событий io_context (блокирующий).
    void run();

   private:
    /// @brief Ожидать SIGINT / SIGTERM и инициировать graceful shutdown.
    awaitable<void> waitForShutdown();

    asio::io_context m_context;
    asio::signal_set m_signals;
    TcpServer m_server;

    OperationalMode m_mode;
};

}  // namespace MillSim
