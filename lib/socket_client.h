// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#include "util.h"

namespace PG {

namespace asio = boost::asio;

class socket_client {
public:
    explicit socket_client(asio::io_context::strand&, string path);
    virtual ~socket_client();

    string send_and_recv(const string& req);

protected:
    asio::io_context::strand& strand_;
    string path_;
    asio::local::stream_protocol::socket socket_;
};

}  // namespace PG
