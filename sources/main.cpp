#include <boost/program_options.hpp>
#include <iostream>

#include "appcore.hpp"

namespace po = boost::program_options;

namespace {

MillSim::OperationalMode parseCommandLine(int argc, char* argv[]) {
    po::options_description desc(
        "MillSim — milling machine simulator server");
    desc.add_options()("help,h", "show help")(
        "emulation", "simulation mode (default: production)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << std::endl;
        std::exit(0);
    }

    if (!vm.contains("emulation")) {
        std::cout
            << "Production mode not implemented yet, switching to --emulation"
            << std::endl;
    }

    return MillSim::OperationalMode::Simulation;
}

}  // namespace

int main(int argc, char* argv[]) {
    MillSim::AppCore app(parseCommandLine(argc, argv));
    app.run();
}
