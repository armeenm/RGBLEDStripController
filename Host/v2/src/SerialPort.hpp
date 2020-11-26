#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

using namespace boost;

class SerialPort {
private:
  std::shared_ptr<asio::io_context> _ctx;
  asio::serial_port _serial_port;
  asio::deadline_timer _timer;
  std::string _port_str;
  bool _read_error;

  int _port_fd;

  void read_complete(const system::error_code& error, size_t bytes_transferred);
  void time_out(const system::error_code& error);

public:
  // Constructor/Destructor
  SerialPort(
             const std::shared_ptr<asio::io_context> ctx = std::make_shared<asio::io_context>(),
             const std::string& port_str = "/dev/ttyUSB0"
             ) :
    _ctx(std::move(ctx)),
    _serial_port(*ctx, port_str),
    _timer(_serial_port.get_io_service()),
    _port_str(port_str) {
    _port_fd = _serial_port.native_handle();
    set_dtr(true);
  }

  ~SerialPort() {
    //set_dtr(true);
    _serial_port.close();
  }

  // Accessors
  std::shared_ptr<asio::io_context> get_ctx();
  const asio::serial_port& get_serial_port();
  int get_native_handle();

  // I/O
  void set_dtr(bool enabled);
  std::optional<std::string> sync_read();
  std::optional<char> async_read_timeout(size_t timeout);
  void send_str(const std::string&);

  // Misc.
  asio::serial_port* operator->();
  // Get rid of copy ctors
  SerialPort(const SerialPort& other) = delete;
  SerialPort& operator=(const SerialPort& other) = delete;
};

#endif
