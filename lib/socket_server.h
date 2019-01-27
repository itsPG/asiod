// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <iostream>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#include "util.h"

namespace PG {

namespace asio = boost::asio;

class socket_server {
public:
    explicit socket_server(asio::io_context::strand&, string path_);
    virtual ~socket_server();

    void listen();

protected:
    virtual void start_session(asio::local::stream_protocol::socket);

    asio::io_context::strand& strand_;
    string path_;
};

}  // namespace PG