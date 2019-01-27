// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "session.h"
#include "socket_server.h"
#include "worker_pool.h"

#include <boost/asio/deadline_timer.hpp>

using namespace PG;

constexpr auto domain_socket_path = "/tmp/PG_asio.sock";

class daemon_session : public session {
public:
    explicit daemon_session(asio::io_context::strand& strand, asio::local::stream_protocol::socket socket,
                            worker_pool& pool)
        : session{strand, std::move(socket)}
        , worker_pool_{pool}
        , timer_{strand_.context()}
    {}

    virtual ~daemon_session() = default;

    string process_command(const string& cmd, asio::yield_context& yield) override
    {
        string resp;

        int32_t task_id = worker_pool_.add_task(
            [&]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                resp = "[Handle Request] " + cmd + "\n";
                asio::post(strand_.context(), [&]() { timer_.cancel(); });
            },
            1);

        cout << "waiting for worker processing " << task_id << endl;
        // allow a task running for 30s
        timer_.expires_after(std::chrono::seconds(30));
        boost::system::error_code ec;
        worker_pool_.check_queue();

        timer_.async_wait(yield[ec]);
        cout << "Done: " << resp << endl;

        return resp;
    }

private:
    worker_pool& worker_pool_;
    boost::asio::steady_timer timer_;
};

class my_daemon : public socket_server {
public:
    my_daemon(asio::io_context::strand& strand, const std::string& path, worker_pool& pool)
        : socket_server{strand, path}
        , worker_pool_{pool}
    {}

    virtual ~my_daemon() = default;

    void start_session(asio::local::stream_protocol::socket socket)
    {
        std::make_shared<daemon_session>(strand_, std::move(socket), worker_pool_)->start();
    }

private:
    worker_pool& worker_pool_;
};

int main()
{
    try {
        asio::io_context io_context;
        asio::io_context::strand strand{io_context};
        worker_pool pool{strand, 8};
        my_daemon d{strand, domain_socket_path, pool};
        io_context.run();
    } catch (const std::exception& e) {
        std::cout << "main error: " << e.what() << std::endl;
    }
}