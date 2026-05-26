#pragma once

namespace MillSim {

/// Максимальное количество одновременных подключений.
constexpr unsigned short CONNECTION_COUNT = 5;

/// TCP-порт, на котором слушает сервер.
constexpr unsigned short PORT = 2809;

/// @brief Режим работы приложения.
enum class OperationalMode { Simulation, Production };

}  // namespace MillSim
