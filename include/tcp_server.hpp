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

/// @brief TCP-сервер, принимающий подключения и отвечающий эхо-ответами.
class TcpServer {
   public:
    /// @brief Конструктор, привязывающий сервер к переданному io_context.
    /// @param pCtx  Указатель на asio::io_context (должен пережить сервер).
    TcpServer(asio::io_context* pCtx);

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    ~TcpServer() = default;

    /// @brief Закрыть acceptor и прекратить приём новых подключений.
    void stop();

   private:
    /// @brief Запустить корутину listen.
    void setupServer();

    /// @brief Цикл принятия входящих подключений.
    /// @details Для каждого принятого сокета порождается корутина echo.
    awaitable<void> listen();

    /// @brief Цикл эхо: прочитать строку, отправить "Response: <line>".
    /// @param socket  Подключённый сокет для чтения и записи.
    awaitable<void> echo(ip::tcp::socket socket);

    asio::io_context* m_context;
    ip::tcp::acceptor m_acceptor;
};

}  // namespace MillSim
