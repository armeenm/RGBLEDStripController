#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

using namespace boost;

class SerialPort {
private:
  std::shared_ptr<asio::io_context> _ctx;
  asio::serial_port _serial_port;
  std::string _port_str;
public:
  SerialPort(
             const std::shared_ptr<asio::io_context> ctx = std::make_shared<asio::io_context>(),
             const std::string& port_str = "/dev/ttyUSB0"
             ) : _ctx(std::move(ctx)), _serial_port(*ctx, port_str), _port_str(port_str) {}

  std::shared_ptr<asio::io_context> get_ctx();

  const asio::serial_port& get_serial_port();

  void send_str(const std::string&);

  asio::serial_port* operator->();

  // Get rid of copy ctors
  SerialPort(const SerialPort& other) = delete;
  SerialPort& operator=(const SerialPort& other) = delete;
};

#endif
