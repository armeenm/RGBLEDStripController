#include <sys/ioctl.h>
#include <boost/bind.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <optional>

#include "SerialPort.hpp"

using namespace boost;

void SerialPort::read_complete(const system::error_code& error, size_t bytes_transferred) {
  _read_error = (error || bytes_transferred == 0);

  _timer.cancel();
}

void SerialPort::time_out(const system::error_code& error) {
  if (error)
    return;
  _serial_port.cancel();
}

std::shared_ptr<asio::io_context> SerialPort::get_ctx() {
  return _ctx;
}

const asio::serial_port& SerialPort::get_serial_port() {
  return _serial_port;
}

int SerialPort::get_native_handle() {
  return _port_fd;
}

asio::serial_port* SerialPort::operator->() {
  return &_serial_port;
}

void SerialPort::set_dtr(bool enabled) {
  int data = TIOCM_DTR;
  if (enabled) {
    std::cout << "Enabling DTR..." << std::endl;
    ioctl(_port_fd, TIOCMBIS, &data);
  } else {
    std::cout << "Disabling DTR..." << std::endl;
    ioctl(_port_fd, TIOCMBIC, &data);
  }
}

std::optional<std::string> SerialPort::sync_read() {
  std::string data;
  system::error_code error;

  _serial_port.read_some(asio::buffer(data), error);

  if (error)
    return {};

  return data;
}

std::optional<char> SerialPort::async_read_timeout(size_t timeout) {
  char c;

  std::cout << "Beginning read..." << std::endl;

  // Port reset required after timeout or cancel
  _serial_port.get_io_service().reset();

  asio::async_read(
                   _serial_port,
                   asio::buffer(&c, 1),
                   bind(
                        &SerialPort::read_complete,
                        this,
                        asio::placeholders::error,
                        asio::placeholders::bytes_transferred
                        )
                   );

  _timer.expires_from_now(posix_time::milliseconds(timeout));
  _timer.async_wait(bind(&SerialPort::time_out, this, asio::placeholders::error));

  _serial_port.get_io_service().run();

  if(!_read_error)
    return c;
  return {};
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
