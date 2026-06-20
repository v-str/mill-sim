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

enum class ClientRole { Admin, Auditor };

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

    awaitable<void> readLoop(std::function<void(std::string)> onReceived);

   private:
    ip::tcp::socket m_socket;
    ClientRole m_role;
};

}  // namespace MillSim
