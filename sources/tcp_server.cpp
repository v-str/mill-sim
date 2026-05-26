#include "tcp_server.hpp"

#include <boost/asio/read_until.hpp>

namespace MillSim {

TcpServer::TcpServer(asio::io_context* pCtx)
    : m_context(pCtx),
      m_acceptor(*m_context, ip::tcp::endpoint(ip::tcp::v4(), MillSim::PORT)) {
    cout << "Server started" << endl;
    setupServer();
}

void TcpServer::setupServer() {
    asio::co_spawn(*m_context, listen(), asio::detached);
}

awaitable<void> TcpServer::listen() {
    while (true) {
        ip::tcp::socket socket =
            co_await m_acceptor.async_accept(asio::use_awaitable);

        co_spawn(m_acceptor.get_executor(), echo(std::move(socket)), detached);
    }
}

awaitable<void> TcpServer::echo(ip::tcp::socket socket) {
    try {
        asio::streambuf buf;
        while (true) {
            co_await asio::async_read_until(socket, buf, '\n',
                                            asio::use_awaitable);

            std::istream is(&buf);
            std::string line;
            std::getline(is, line);

            std::string response = "server response: " + line + '\n';
            co_await async_write(socket, asio::buffer(response),
                                 asio::use_awaitable);
        }
    } catch (const boost::system::system_error& e) {
        std::cout << "echo ended: " << e.what() << std::endl;
    }
}

}  // namespace MillSim
