#include "client_session.hpp"

#include <boost/asio/read_until.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>

namespace CNC {

ClientSession::ClientSession(ip::tcp::socket socket, ClientRole role)
    : m_socket(std::move(socket)), m_role(role) {}

ip::tcp::endpoint ClientSession::remoteEndpoint() const {
    boost::system::error_code ec;
    auto ep = m_socket.remote_endpoint(ec);
    return ep;
}

void ClientSession::disconnect() {
    boost::system::error_code ec;
    m_socket.close(ec);
}

// вот тут уже блокируемся и работаем с клиентом, если мы отсюда вышли, значит
// сессия завершилась
awaitable<void> ClientSession::readLoop(
    std::function<void(std::string)> onReceivedCallback) {
    try {
        asio::streambuf buf;
        while (true) {
            co_await asio::async_read_until(m_socket, buf, '\n',
                                            asio::use_awaitable);

            std::istream is(&buf);
            std::string line;
            std::getline(is, line);

            // если админ и колбек есть
            if (isAdmin() && onReceivedCallback) {
                onReceivedCallback(line);
            }
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() != asio::error::eof) {
            std::cerr << "read error: " << e.code().message() << std::endl;
        }
    }
}

}  // namespace CNC
