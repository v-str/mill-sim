#include "tcp_server.hpp"

#include <systemd/sd-journal.h>

#include <boost/asio/read_until.hpp>

namespace MillSim {

TcpServer::TcpServer(asio::io_context* pCtx)
    : m_context(pCtx),
      m_acceptor(*m_context, ip::tcp::endpoint(ip::tcp::v4(), MillSim::PORT)) {
    sd_journal_print(LOG_INFO, "Server started on port %u", MillSim::PORT);
    setupServer();
}

void TcpServer::stop() {
    boost::system::error_code ec;
    m_acceptor.close(ec);
    if (ec) {
        sd_journal_print(LOG_ERR, "Error closing acceptor: %s",
                         ec.message().c_str());
    }
}

void TcpServer::setupServer() {
    asio::co_spawn(*m_context, listen(), asio::detached);
}

awaitable<void> TcpServer::listen() {
    try {
        while (true) {
            ip::tcp::socket socket =
                co_await m_acceptor.async_accept(asio::use_awaitable);

            auto ep = socket.remote_endpoint();
            sd_journal_print(LOG_INFO, "New connection, client: %s",
                             ep.address().to_string().c_str());

            co_spawn(m_acceptor.get_executor(), echo(std::move(socket)),
                     detached);
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() == asio::error::operation_aborted) {
            sd_journal_print(LOG_INFO, "Server stopped");
        } else {
            sd_journal_print(LOG_ERR, "Listen error: %s", e.what());
        }
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

            std::string response = "Echo: " + line + '\n';
            co_await async_write(socket, asio::buffer(response),
                                 asio::use_awaitable);
            sd_journal_print(LOG_WARNING, response.c_str());
        }
    } catch (const boost::system::system_error& e) {
        auto code = e.code();
        if (code != asio::error::eof && code != asio::error::connection_reset) {
            sd_journal_print(LOG_ERR, "Echo error: %s", e.what());
        }
    }
}

}  // namespace MillSim
