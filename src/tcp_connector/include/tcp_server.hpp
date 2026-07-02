#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "client_session.hpp"

namespace CNC {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;

/**
 * @brief Асинхронный TCP-сервер (Boost.Asio), связывающий ROS 2 с
 *        Android-приложением (3-осевой симулятор ЧПУ).
 *
 * Один io_context::run() в std::jthread обрабатывает три асинхронные цепочки:
 *   - Accept (listen) — async_accept новых клиентов
 *   - Watch (watchSession) — readLoop от клиентов на strand
 *   - Write (send/doWrite) — strand-очередь широковещательной рассылки
 *
 * Strand гарантирует последовательный доступ к m_sessions и m_writeQueue.
 * send() вызывает asio::post на strand — её можно вызывать из любого потока.
 */
class TcpServer {
   public:
    TcpServer() = default;
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    /**
     * @brief Запустить сервер на порту port, не более connectionCount сессий.
     *
     * Создаёт io_context, acceptor, spawn-ит listen(), запускает поток
     * m_asioThread (std::jthread).
     */
    void runServer(unsigned short port, unsigned short connectionCount);

    /**
     * @brief Остановить сервер: закрыть сессии, io_context, acceptor.
     *
     * После stop() можно повторно вызвать runServer().
     */
    void stop();

    /**
     * @brief Установить колбэк для сообщений от Admin-сессии.
     *
     * Колбэк исполняется в readLoop на m_asioThread.
     */
    void setOnMessageReceivedCallback(
        std::function<void(std::string)> callback);

    /**
     * @brief Установить колбэк при подключении клиента.
     *
     * Вызывается на strand после добавления сессии в m_sessions.
     */
    void setOnClientConnectedCallback(
        std::function<void(std::shared_ptr<ClientSession>)> callback);

    /**
     * @brief Установить колбэк при отключении клиента.
     *
     * Вызывается на strand после readLoop (до удаления сессии).
     */
    void setOnClientDisconnectedCallback(
        std::function<void(std::shared_ptr<ClientSession>)> callback);

    /**
     * @brief Поставить сообщение в очередь broadcast-рассылки всем сессиям.
     *
     * Потокобезопасна: asio::post на strand. Если очередь была пуста —
     * немедленно запускает doWrite().
     */
    void send(std::string data);

   private:
    /// Цикл async_accept. При превышении connectionCount шлёт клиенту
    /// "ERROR: max connections reached (N)" и закрывает сокет.
    awaitable<void> listen();

    /// Блокируется на readLoop() сессии; при отключении удаляет её из
    /// m_sessions и, если ушёл Admin, назначает Admin-ом первую из оставшихся.
    awaitable<void> watchSession(std::shared_ptr<ClientSession> session);

    /// Берёт первый элемент m_writeQueue, async_write на каждую сессию,
    /// ждёт завершения всех (shared counter), затем вызывает себя для
    /// следующего элемента.
    void doWrite();

    std::function<void(std::string)> m_onMessageReceivedCallback;
    std::function<void(std::shared_ptr<ClientSession>)>
        m_onClientConnectedCallback;
    std::function<void(std::shared_ptr<ClientSession>)>
        m_onClientDisconnectedCallback;
    std::unique_ptr<asio::io_context> m_ioContext;
    std::unique_ptr<asio::strand<asio::io_context::executor_type>> m_strand;
    unsigned short m_connectionCount;
    std::unique_ptr<ip::tcp::acceptor> m_acceptor;
    std::jthread m_asioThread;
    std::vector<std::shared_ptr<ClientSession>> m_sessions;
    std::deque<std::string> m_writeQueue;
};

}  // namespace CNC
