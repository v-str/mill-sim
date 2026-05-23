#include "mill_machine.hpp"

int main() {
  mill::MillMachine machine(mill::OperationalMode::Simulation);
  machine.start();
}
