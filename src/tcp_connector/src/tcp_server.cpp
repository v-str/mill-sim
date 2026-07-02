#include "tcp_server.hpp"

#include <iostream>

namespace CNC {

// ---------------------------------------------------------------------------
TcpServer::~TcpServer() { stop(); }

// ---------------------------------------------------------------------------
// Запуск и остановка
// ---------------------------------------------------------------------------

void TcpServer::runServer(unsigned short port, unsigned short connectionCount) {
    m_connectionCount = connectionCount;
    m_ioContext = std::make_unique<asio::io_context>();
    m_strand = std::make_unique<asio::strand<asio::io_context::executor_type>>(
        asio::make_strand(*m_ioContext));
    m_acceptor = std::make_unique<ip::tcp::acceptor>(
        *m_ioContext, ip::tcp::endpoint(ip::tcp::v4(), port));

    co_spawn(*m_ioContext, listen(), asio::detached);
    m_asioThread = std::jthread([this] { m_ioContext->run(); });
}

void TcpServer::stop() {
    for (auto& session : m_sessions) {
        session->disconnect();
    }
    m_sessions.clear();

    if (m_ioContext) {
        m_ioContext->stop();
    }
    if (m_acceptor) {
        boost::system::error_code ec;
        m_acceptor->close(ec);
    }
}

// ---------------------------------------------------------------------------
// Принятие подключений
// ---------------------------------------------------------------------------

awaitable<void> TcpServer::listen() {
    try {
        while (true) {
            ip::tcp::socket socket =
                co_await m_acceptor->async_accept(asio::use_awaitable);

            if (m_sessions.size() >= m_connectionCount) {
                // Обязательно co_await async_write — close() оборвёт
                // незавершённую запись, и клиент не успеет прочитать ошибку.
                auto msg = std::string("ERROR: max connections reached (") +
                           std::to_string(m_connectionCount) + ")\n";
                co_await async_write(socket, asio::buffer(msg),
                                     asio::use_awaitable);
                socket.close();
                continue;
            }

            auto role =
                m_sessions.empty() ? ClientRole::Admin : ClientRole::Auditor;
            auto session =
                std::make_shared<ClientSession>(std::move(socket), role);
            m_sessions.push_back(session);

            if (m_onClientConnectedCallback) {
                m_onClientConnectedCallback(session);
            }

            co_spawn(*m_strand, watchSession(session), detached);
        }
    } catch (const boost::system::system_error& e) {
        std::cerr << "listen error: " << e.code().message() << std::endl;
    }
}

// ---------------------------------------------------------------------------
// Наблюдение за сессией
// ---------------------------------------------------------------------------

awaitable<void> TcpServer::watchSession(
    std::shared_ptr<ClientSession> session) {
    // блочим сессию, пока она не отключится
    co_await session->readLoop(m_onMessageReceivedCallback);

    if (m_onClientDisconnectedCallback) {
        m_onClientDisconnectedCallback(session);
    }

    // удаляем завершившуюся сессию
    auto it = std::find_if(m_sessions.begin(), m_sessions.end(),
                           [&](auto& s) { return s == session; });
    if (it != m_sessions.end()) {
        bool wasAdmin = (*it)->isAdmin();
        m_sessions.erase(it);

        if (wasAdmin && !m_sessions.empty()) {
            m_sessions.front()->changeRole(ClientRole::Admin);
        }
    }
}

// ---------------------------------------------------------------------------
// Отправка (post на strand) и широковещательная запись
// ---------------------------------------------------------------------------

void TcpServer::send(std::string data) {
    asio::post(*m_strand, [this, data = std::move(data)] {
        m_writeQueue.push_back(std::move(data));
        if (m_writeQueue.size() == 1) {
            doWrite();
        }
    });
}

void TcpServer::doWrite() {
    if (m_writeQueue.empty()) return;

    // shared_ptr — async_write может пережить этот вызов.
    auto data = std::make_shared<std::string>(std::move(m_writeQueue.front()));
    m_writeQueue.pop_front();

    auto counter = std::make_shared<size_t>(m_sessions.size());
    if (*counter == 0) {
        doWrite();
        return;
    }

    // Параллельный async_write на каждую сессию. Колбэки на strand.
    for (auto& session : m_sessions) {
        async_write(
            session->socket(), asio::buffer(*data),
            asio::bind_executor(
                *m_strand, [this, counter](boost::system::error_code /*ec*/,
                                           size_t /*bytes*/) {
                    // Когда все сессии завершили запись — следующий элемент.
                    if (--(*counter) == 0) {
                        doWrite();
                    }
                }));
    }
}

// ---------------------------------------------------------------------------
void TcpServer::setOnMessageReceivedCallback(
    std::function<void(std::string)> callback) {
    m_onMessageReceivedCallback = callback;
}

void TcpServer::setOnClientConnectedCallback(
    std::function<void(std::shared_ptr<ClientSession>)> callback) {
    m_onClientConnectedCallback = callback;
}

void TcpServer::setOnClientDisconnectedCallback(
    std::function<void(std::shared_ptr<ClientSession>)> callback) {
    m_onClientDisconnectedCallback = callback;
}

}  // namespace CNC
