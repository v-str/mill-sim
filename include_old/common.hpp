#pragma once

namespace MillSim {

/// Maximum number of simultaneous connections.
constexpr unsigned short CONNECTION_COUNT = 5;

/// TCP port the server listens on.
constexpr unsigned short PORT = 2809;

/// @brief Application operational mode.
enum class OperationalMode { Simulation, Production };

}  // namespace MillSim
