#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

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
    void setOnMessageSentCallback(std::function<void(std::string)> callback);
    void send(std::string data);

   private:
    void setupServer();
    awaitable<void> listen();
    awaitable<void> echo(ip::tcp::socket socket);

    std::function<void(std::string)> m_onMessageCallback;
    std::function<void(std::string)> m_onMessageSentCallback;
    std::unique_ptr<asio::io_context> m_ioContext;
    unsigned short m_connectionCount;
    std::unique_ptr<ip::tcp::acceptor> m_acceptor;
    std::thread m_asioThread;
};

}  // namespace MillSim
