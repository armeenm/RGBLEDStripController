#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <optional>

#include "sendcolorsv2.hpp"

using namespace boost;

int main(int argc, char** argv) {
  // Options
  const std::string SERIAL_PORT = "/dev/ttyUSB1";
  const asio::serial_port_base::baud_rate BAUD_RATE(9600);
  const asio::serial_port_base::character_size CHAR_SIZE(8);
  const asio::serial_port_base::flow_control FLOW(asio::serial_port_base::flow_control::none);
  const asio::serial_port_base::parity PARITY(asio::serial_port_base::parity::none);
  const asio::serial_port_base::stop_bits STOP_BITS(asio::serial_port_base::stop_bits::one);

  asio::io_context ctx;
  std::unique_ptr<asio::serial_port> port;
  std::string serial_port_str;
  std::string cmd;

  switch(argc) {
  case 1:
    return -1;
    break;

  case 2:
    serial_port_str = SERIAL_PORT;
    cmd = argv[1];
    break;

  case 3:
    serial_port_str = argv[1];
    cmd = argv[2];
    break;
  }

  // Initialize serial port
  try {
    port = init_serial(ctx, serial_port_str);
  } catch (std::exception& e) {
    std::cout << "Unable to open serial port!" << std::endl;
    return -1;
  }

  // Set options
  port->set_option(BAUD_RATE);
  port->set_option(CHAR_SIZE);
  port->set_option(FLOW);
  port->set_option(PARITY);
  port->set_option(STOP_BITS);

  // Write to port
  try {
    send_str(move(port), cmd);
  } catch (const std::exception& e) {
    std::cout << "An exception occurred while trying to write." << std::endl;
    return -1;
  }

  return 0;
}

std::unique_ptr<asio::serial_port> init_serial(asio::io_context& ctx, const std::string& serial_port_str) {
  std::unique_ptr<asio::serial_port> port_ptr =
    std::make_unique<asio::serial_port>(asio::serial_port(ctx, serial_port_str));

  if (!port_ptr || !port_ptr->is_open())
    throw -1;

  return port_ptr;
}

void send_str(std::unique_ptr<asio::serial_port> port, const std::string& data) {
  try {
    if (!port || !port->is_open()) {
      std::cout << "Port not opened!" << std::endl;
      throw -2;
    }

    system::error_code error;

    asio::write(
                *port,
                boost::asio::buffer(data.c_str(), data.size()),
                asio::transfer_at_least(data.size()),
                error
                );

    if (error) {
      std::cout << "ASIO write failed: " << error.message() << std::endl;
      throw -3;
    }

  } catch (const std::exception& e) {
    throw -1;
  }
}
