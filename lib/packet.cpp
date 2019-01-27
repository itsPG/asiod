// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "packet.h"

#include <chrono>
#include <cstdio>
#include <thread>

#include <boost/asio/read.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>

namespace PG {

void async_write_packet(asio::local::stream_protocol::socket& socket, const string& packet, asio::yield_context& yield)
{
    const uint32_t length = packet.size();

    asio::async_write(socket, asio::buffer(&length, kHeaderSize), yield);
    asio::async_write(socket, asio::buffer(packet), yield);
}

string async_read_packet(asio::local::stream_protocol::socket& socket, asio::yield_context& yield)
{
    std::array<char, kHeaderSize> header_buf;

    asio::async_read(socket, asio::buffer(header_buf, kHeaderSize), asio::transfer_exactly(kHeaderSize), yield);

    const uint32_t length{*reinterpret_cast<uint32_t*>(header_buf.data())};
    std::vector<char> buf(length);

    asio::async_read(socket, asio::buffer(buf, length), yield);

    return {buf.begin(), buf.end()};
}

}  // namespace PG