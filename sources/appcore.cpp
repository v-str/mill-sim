#include "appcore.hpp"

MillSim::AppCore::AppCore(OperationalMode mode)
    : m_mode(mode), m_server(&m_context) {}

void MillSim::AppCore::run() { m_context.run(); }
