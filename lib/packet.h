// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>

#include "util.h"

namespace PG {

namespace asio = boost::asio;

static constexpr std::size_t kHeaderSize = sizeof(uint32_t);

string async_read_packet(asio::local::stream_protocol::socket& socket, asio::yield_context& yield);
void async_write_packet(asio::local::stream_protocol::socket& socket, const string& packet, asio::yield_context& yield);

}  // namespace PG