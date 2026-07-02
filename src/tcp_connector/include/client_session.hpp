#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

namespace CNC {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using asio::awaitable;
using asio::co_spawn;
using asio::detached;

/**
 * @brief Роли клиентских сессий TCP-сервера.
 *
 * Admin — может отправлять команды на через onMessageReceivedCallback.
 * Auditor — только получает широковещательные данные, его ввод игнорируется.
 *
 * Роль Admin назначается первому подключившемуся клиенту; при его отключении
 * Admin становится первый Auditor в пуле TcpServer.
 */
enum class ClientRole { Admin, Auditor };

/**
 * @brief Сессия одного клиента: сокет + роль + readLoop.
 */
class ClientSession {
   public:
    ClientSession(ip::tcp::socket socket, ClientRole role);

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    ClientSession(ClientSession&&) = default;
    ClientSession& operator=(ClientSession&&) = default;

    ip::tcp::socket& socket() { return m_socket; }

    ClientRole role() const { return m_role; }
    bool isAdmin() const { return m_role == ClientRole::Admin; }

    ip::tcp::endpoint remoteEndpoint() const;
    void changeRole(ClientRole role) { m_role = role; }
    void disconnect();

    /**
     * @brief Цикл чтения строк из сокета (блокирующая корутина).
     *
     * Читает до '\n'; если сессия Admin, передаёт прочитанную строку в
     * onReceivedCallback. Auditor читает и отбрасывает.
     *
     * Завершается при отключении клиента, ошибке чтения или вызове
     * disconnect().
     */
    awaitable<void> readLoop(
        std::function<void(std::string)> onReceivedCallback);

   private:
    ip::tcp::socket m_socket;
    ClientRole m_role;
};

}  // namespace CNC
