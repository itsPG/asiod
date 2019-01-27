// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "socket_server.h"

#include <cstdio>

#include "packet.h"
#include "session.h"

namespace PG {

socket_server::socket_server(asio::io_context::strand& strand, string path)
    : strand_{strand}
    , path_{std::move(path)}
{
    std::remove(path_.c_str());
    listen();
}

socket_server::~socket_server()
{
    std::remove(path_.c_str());
}

void socket_server::listen()
{
    cout << "socket_server::listen()" << endl;

    asio::spawn(strand_, [&](asio::yield_context yield) {
        asio::local::stream_protocol::acceptor acceptor{strand_.context(),
                                                        asio::local::stream_protocol::endpoint{path_}};
        asio::local::stream_protocol::socket socket{strand_.context()};

        while (true) {
            boost::system::error_code error;
            acceptor.async_accept(socket, yield[error]);
            if (error) {
                cout << "accept error" << endl;
            } else {
                cout << "accept one connection" << endl;
                start_session(std::move(socket));
            }
        }
    });
}

void socket_server::start_session(asio::local::stream_protocol::socket socket)
{
    std::make_shared<session>(strand_, std::move(socket))->start();
}

}  // namespace PG
