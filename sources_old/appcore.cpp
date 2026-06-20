#include "appcore.hpp"

#include <systemd/sd-journal.h>

#include <boost/asio/use_awaitable.hpp>

namespace MillSim {

AppCore::AppCore(OperationalMode mode)
    : m_mode(mode),
      m_signals(m_context, SIGINT, SIGTERM),
      m_tcpServer(&m_context) {
    co_spawn(m_context, waitForShutdown(), detached);
}

awaitable<void> AppCore::waitForShutdown() {
    auto sig = co_await m_signals.async_wait(asio::use_awaitable);
    sd_journal_print(LOG_INFO, "Received signal %d, shutting down...", sig);
    m_tcpServer.stop();
    m_context.stop();
}

void AppCore::run() { m_context.run(); }

}  // namespace MillSim
