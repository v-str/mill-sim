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

/// @brief TCP server that accepts connections and echoes replies.
class TcpServer {
   public:
    /// @brief Constructor binding the server to the given io_context.
    /// @param pCtx  Pointer to asio::io_context (must outlive the server).
    TcpServer(asio::io_context* pCtx);

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    ~TcpServer() = default;

    /// @brief Close the acceptor and stop accepting new connections.
    void stop();

   private:
    /// @brief Spawn the listen coroutine.
    void setupServer();

    /// @brief Accept incoming connections loop.
    /// @details For each accepted socket, spawns an echo coroutine.
    awaitable<void> listen();

    /// @brief Echo loop: read a line, send back "Echo: <line>".
    /// @param socket  Connected socket for reading and writing.
    awaitable<void> echo(ip::tcp::socket socket);

    asio::io_context* m_context;
    ip::tcp::acceptor m_acceptor;
};

}  // namespace MillSim
