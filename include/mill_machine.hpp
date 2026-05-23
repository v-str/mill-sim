#pragma once

#include <boost/asio.hpp>

#include "common.hpp"

namespace mill {

namespace asio = boost::asio;

class MillMachine {
 public:
  MillMachine(OperationalMode mode) : m_mode(mode) {}

  void start();

 private:
  OperationalMode m_mode;
  asio::io_context m_context;
};

}  // namespace mill
