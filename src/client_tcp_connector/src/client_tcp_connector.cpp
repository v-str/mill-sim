#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include "tcp_server.hpp"

namespace MillSim {

class ClientTcpConnector : public rclcpp::Node {
   public:
    ClientTcpConnector(const rclcpp::NodeOptions& options)
        : Node("client_tcp_connector", options) {
        auto port = static_cast<unsigned short>(get_parameter("port").as_int());
        auto connectionCount = static_cast<unsigned short>(
            get_parameter("connection_count").as_int());

        setup();

        m_tcpServer.runServer(port, connectionCount);
    }

   private:
    void setup() {
        if (get_parameter("operational_mode").as_string() == "simulation") {
            m_tcpServer.setOnMessageReceivedCallback([this](std::string msg) {
                RCLCPP_INFO(get_logger(), "(SIMULATION) TCP получено: %s",
                            msg.c_str());
            });

            m_tcpTxSub = create_subscription<std_msgs::msg::String>(
                "tcp_tx", 10, [this](std_msgs::msg::String::SharedPtr msg) {
                    RCLCPP_INFO(get_logger(), "(SIMULATION) TCP отправлено: %s",
                                msg->data.c_str());
                    m_tcpServer.send(msg->data + '\n');
                });
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr m_tcpTxSub;
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
