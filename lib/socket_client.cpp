// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "socket_client.h"

#include <cstdio>

#include <boost/asio/spawn.hpp>

#include "packet.h"

namespace PG {

namespace asio = boost::asio;

socket_client::socket_client(asio::io_context::strand& strand, string path)
    : strand_{strand}
    , path_{std::move(path)}
    , socket_{strand_.context()}
{
    socket_.connect(asio::local::stream_protocol::endpoint{path_});
}

socket_client::~socket_client() {}

string socket_client::send_and_recv(const string& req)
{
    string resp;

    asio::spawn(strand_, [&](asio::yield_context yield) {
        async_write_packet(socket_, req, yield);
        resp = async_read_packet(socket_, yield);
        cout << getpid() << " [Client] " << resp << endl;
    });

    return resp;
}

}  // namespace PG