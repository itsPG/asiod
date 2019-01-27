// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "session.h"

#include <thread>

#include <boost/asio/deadline_timer.hpp>

#include "packet.h"
#include "util.h"

namespace PG {

namespace asio = boost::asio;

session::session(asio::io_context::strand& strand, asio::local::stream_protocol::socket socket)
    : std::enable_shared_from_this<session>{}
    , strand_{strand}
    , socket_{std::move(socket)}
    , timer_{strand_.context()}
{}

session::~session()
{
    cout << "session::~session()" << endl;
}

void session::start()
{
    asio::spawn(strand_, [self = shared_from_this()](asio::yield_context yield) {
        try {
            while (true) {
                self->run(yield);
            }
        } catch (const boost::system::system_error& e) {
            cout << "session start failed: " << e.what() << endl;
        } catch (const std::runtime_error& e) {
            cout << "session runtime error: " << e.what() << endl;
        }
    });
}

void session::run(asio::yield_context& yield)
{
    string req{async_read_packet(socket_, yield)};
    string resp{process_command(req, yield)};

    async_write_packet(socket_, resp, yield);
}

string session::process_command(const string& cmd, asio::yield_context& yield)
{
    string resp;

    // simulate a task that would take some time
    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        resp = "[Handled] " + cmd;
        asio::post(strand_.context(), [&]() { timer_.cancel(); });
    });

    // allow a task running for 30s
    timer_.expires_after(std::chrono::seconds(30));
    boost::system::error_code ec;
    timer_.async_wait(yield[ec]);
    t.join();

    cout << "[session::process_command base] " << resp << endl;
    return resp;
}

}  // namespace PG
