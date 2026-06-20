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

        setupConnector();

        m_tcpServer.runServer(port, connectionCount);
    }

   private:
    void setupConnector() {
        if (get_parameter("operational_mode").as_string() == "simulation") {
            m_tcpServer.setOnMessageReceivedCallback([this](std::string msg) {
                // этот колбек будет вызываться при получении сообщения из
                // Андроид-приложения, но пока тут просто лог
                RCLCPP_INFO(get_logger(), "(SIMULATION) TCP получено: %s",
                            msg.c_str());
            });
        }
    }

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
