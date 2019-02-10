#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <optional>

#include "SerialPort.hpp"
#include "sendcolorsv2.hpp"

using namespace boost;

int main(int argc, char** argv) {
  // Options
  const asio::serial_port_base::baud_rate BAUD_RATE(9600);
  const asio::serial_port_base::character_size CHAR_SIZE(8);
  const asio::serial_port_base::flow_control FLOW(asio::serial_port_base::flow_control::none);
  const asio::serial_port_base::parity PARITY(asio::serial_port_base::parity::none);
  const asio::serial_port_base::stop_bits STOP_BITS(asio::serial_port_base::stop_bits::one);

  std::string serial_port_str;
  std::string cmd;

  switch(argc) {
  case 1:
    return -1;
    break;

  case 2:
    cmd = argv[1];
    break;

  case 3:
    serial_port_str = argv[1];
    cmd = argv[2];
    break;
  }

  // Initialize serial port
  SerialPort port;

  // Set options
  port->set_option(BAUD_RATE);
  port->set_option(CHAR_SIZE);
  port->set_option(FLOW);
  port->set_option(PARITY);
  port->set_option(STOP_BITS);

  // Write to port
  try {
    port.send_str(cmd);
  } catch (const std::exception& e) {
    std::cout << "An exception occurred while trying to write." << std::endl;
    return -1;
  }

  return 0;
}
