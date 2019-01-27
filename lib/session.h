// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/spawn.hpp>

#include "util.h"

namespace PG {

namespace asio = boost::asio;

class session : public std::enable_shared_from_this<session> {
public:
    explicit session(asio::io_context::strand&, asio::local::stream_protocol::socket);
    virtual ~session();

    void start();
    void run(asio::yield_context&);
    virtual string process_command(const string& cmd, asio::yield_context&);

protected:
    asio::io_context::strand& strand_;
    asio::local::stream_protocol::socket socket_;
    asio::steady_timer timer_;
};

}  // namespace PG
