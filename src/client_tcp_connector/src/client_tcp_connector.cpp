#include <rclcpp/rclcpp.hpp>

#include "tcp_server.hpp"

namespace MillSim {

class ClientTcpConnector : public rclcpp::Node {
   public:
    ClientTcpConnector(const rclcpp::NodeOptions& options)
        : Node("client_tcp_connector", options) {
        auto port = static_cast<unsigned short>(get_parameter("port").as_int());
        auto connectionCount = static_cast<unsigned short>(
            get_parameter("connection_count").as_int());

        m_tcpServer.setOnMessage([this](std::string msg) {
            RCLCPP_INFO(get_logger(), "TCP получено: %s", msg.c_str());
        });

        m_tcpServer.runServer(port, connectionCount);
    }

   private:
    TcpServer m_tcpServer;
};

}  // namespace MillSim

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    rclcpp::NodeOptions opts;
    opts.automatically_declare_parameters_from_overrides(true);

    auto node = std::make_shared<MillSim::ClientTcpConnector>(opts);
    rclcpp::spin(node);
    rclcpp::shutdown();
}
