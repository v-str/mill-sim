#include "tcp_server.hpp"

#include <boost/asio/read_until.hpp>
#include <systemd/sd-journal.h>

namespace MillSim {

TcpServer::TcpServer(asio::io_context* pCtx)
    : m_context(pCtx),
      m_acceptor(*m_context, ip::tcp::endpoint(ip::tcp::v4(), MillSim::PORT)) {
    sd_journal_print(LOG_INFO, "Server started on port %u", MillSim::PORT);
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
        sd_journal_print(LOG_INFO, "echo ended: %s", e.what());
    }
}

}  // namespace MillSim
