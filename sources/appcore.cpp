#include "appcore.hpp"

MillSim::AppCore::AppCore(OperationalMode mode)
    : m_mode(mode), m_server(&m_context) {
    // setupApp();
}

void MillSim::AppCore::run() { m_context.run(); }

void MillSim::AppCore::setupApp() {
    while (true) {
        // TcpServer уже работает
        // Если произошло подключение, т.е.
    }
}
