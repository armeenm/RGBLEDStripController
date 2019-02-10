#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <string>
#include <iostream>

#include "SerialPort.hpp"

using namespace boost;

std::shared_ptr<asio::io_context> SerialPort::get_ctx() {
  return _ctx;
}

const asio::serial_port& SerialPort::get_serial_port() {
  return _serial_port;
}

asio::serial_port* SerialPort::operator->() {
  return &_serial_port;
}

void SerialPort::send_str(const std::string& data) {
  system::error_code error;

  asio::write(
              _serial_port,
              asio::buffer(data),
              asio::transfer_at_least(data.size()),
              error
              );

  if (error) {
    std::cout << "ASIO write failed: " << error.message() << std::endl;
    throw -3;
  }
}
