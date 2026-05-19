#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <source_location>
#include <string>
#include <thread>

namespace ba = boost::asio;

void handle_func(ba::ip::tcp::socket socket) {
  try {
    char data[1024];
    while (true) {
      boost::system::error_code ec;
      auto len = socket.read_some(boost::asio::buffer(data), ec);

      if (ec == boost::asio::error::eof) {
        std::cerr << "client disconnected" << std::endl;
      } else if (ec) {
        std::cerr << "generic error" << std::endl;
      }

      std::cout << "received: " << len << " bytes" << std::endl;
      std::cout.write(data, len) << std::endl;

      std::string echo_str("echo: ");
      echo_str.append(data);
      echo_str.append("\n");

      ba::write(socket, ba::buffer(echo_str.data(), echo_str.length()));
      echo_str.clear();
    }
  } catch (std::exception& e) {
    std::source_location loc = std::source_location::current();
    std::cerr << "exception in " << loc.function_name() << ", " << e.what()
              << std::endl;
    std::cout << "connection lost" << std::endl;
  }
}

int main() {
  try {
    ba::io_context ioctx;

    ba::ip::tcp::acceptor acceptor(
        ioctx, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), 2809));

    std::cout << "Server starting at port 2809 ..." << std::endl;

    while (true) {
      ba::ip::tcp::socket socket = acceptor.accept();

      std::cout << "new connection!" << std::endl;

      std::jthread client_thread(handle_func, std::move(socket));

      client_thread.detach();
    }

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
