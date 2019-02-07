#ifndef __SENDCOLORS__
#define __SENDCOLORS__

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <string>
#include <optional>

using namespace boost;

std::unique_ptr<asio::serial_port>
init_serial(asio::io_context& io_ctx, const std::string& serial_port_str);

#endif
