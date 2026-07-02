#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include "tcp_server.hpp"

namespace CNC {

/**
 * @brief ROS 2-нода-мост: временные подписка /tcp_tx → TcpServer::send().
 *
 * В режиме "simulation" подписывается на топик std_msgs/String "tcp_tx" и
 * передаёт каждое сообщение в TCP-сервер (broadcast всем Android-клиентам).
 * В режиме "production" будет также обрабатывать входящие сообщения от
 * Admin-сессии и публиковать их в ROS 2.
 *
 * Параметры (из YAML): port, connection_count, operational_mode.
 */
class TcpConnector : public rclcpp::Node {
   public:
    TcpConnector(const rclcpp::NodeOptions& options)
        : Node("tcp_connector", options) {
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

            m_tcpServer.setOnClientConnectedCallback([this](auto session) {
                RCLCPP_INFO(
                    get_logger(), "Клиент подключился: %s:%d",
                    session->remoteEndpoint().address().to_string().c_str(),
                    session->remoteEndpoint().port());
            });

            m_tcpServer.setOnClientDisconnectedCallback([this](auto session) {
                RCLCPP_INFO(
                    get_logger(), "Клиент отключился: %s:%d",
                    session->remoteEndpoint().address().to_string().c_str(),
                    session->remoteEndpoint().port());
            });

            // тестовая подписка на топик /tcp_tx
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

}  // namespace CNC

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    rclcpp::NodeOptions opts;
    opts.automatically_declare_parameters_from_overrides(true);

    auto node = std::make_shared<CNC::TcpConnector>(opts);
    rclcpp::spin(node);
    rclcpp::shutdown();
}
