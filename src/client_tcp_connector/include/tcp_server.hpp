#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "client_session.hpp"

namespace MillSim {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;

class TcpServer {
   public:
    TcpServer() = default;
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void runServer(unsigned short port, unsigned short connectionCount);
    void stop();

    void setOnMessageReceivedCallback(
        std::function<void(std::string)> callback);
    void send(std::string data);

   private:
    awaitable<void> listen();
    awaitable<void> watchSession(std::shared_ptr<ClientSession> session);
    void doWrite();

    std::function<void(std::string)> m_onMessageReceivedCallback;
    std::unique_ptr<asio::io_context> m_ioContext;
    std::unique_ptr<asio::strand<asio::io_context::executor_type>> m_strand;
    unsigned short m_connectionCount;
    std::unique_ptr<ip::tcp::acceptor> m_acceptor;
    std::thread m_asioThread;

    std::vector<std::shared_ptr<ClientSession>> m_sessions;
    std::deque<std::string> m_writeQueue;
};

}  // namespace MillSim
