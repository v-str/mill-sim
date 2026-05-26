#include "appcore.hpp"

int main() {
    MillSim::AppCore app(MillSim::OperationalMode::Simulation);
    app.run();
}
