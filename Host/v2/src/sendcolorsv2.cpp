#include "sendcolorsv2.hpp"

#include <termios.h>

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "SerialPort.hpp"

using namespace boost;

SerialPort port;

int main(int argc, char** argv) {
  // Options
  const asio::serial_port_base::baud_rate BAUD_RATE(9600);
  const asio::serial_port_base::character_size CHAR_SIZE(8);
  const asio::serial_port_base::flow_control FLOW(
      asio::serial_port_base::flow_control::none);
  const asio::serial_port_base::parity PARITY(
      asio::serial_port_base::parity::none);
  const asio::serial_port_base::stop_bits STOP_BITS(
      asio::serial_port_base::stop_bits::one);

  std::string serial_port_str;
  std::string cmd;

  switch (argc) {
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
  // SerialPort port;

  // Set options
  port->set_option(BAUD_RATE);
  port->set_option(CHAR_SIZE);
  port->set_option(FLOW);
  port->set_option(PARITY);
  port->set_option(STOP_BITS);

  termios oldtio;

  int fd = port.get_native_handle();
  fcntl(fd, F_SETFL, 0);
  termios newtio;
  tcgetattr(fd, &oldtio);
  bzero(&newtio, sizeof(newtio));
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 2;
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  pthread_t reader;

  // Write to port
  pthread_create(&reader, NULL, read_serial, NULL);
  port.send_str(cmd);
  pthread_join(reader, NULL);
  /*
  try {
  } catch (const std::exception& e) {
    std::cout << "An exception occurred while trying to write." << std::endl;
    return -1;
  }
  */

  tcsetattr(fd, TCSANOW, &oldtio);

  return 0;
}

void* read_serial(void* vargp) {
  read(port.get_native_handle(), NULL, 1);
  /*
  std::optional<std::string> data = port.sync_read();
    if (data)
    std::cout << "Read data: " << data << std::endl;
    else
    std::cout << "Failed to read data!" << std::endl;
  */
  return {};
}
