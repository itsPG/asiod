// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <functional>
#include <queue>

#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/steady_timer.hpp>

#include "task.h"
#include "util.h"

namespace PG {

namespace asio = boost::asio;

class task_manager {
public:
    using queue_t = std::priority_queue<task, vector<task>, std::function<bool(const task&, const task&)>>;

    explicit task_manager(asio::io_context::strand& main_strand, asio::io_context& pool_io, int32_t concurrency);
    virtual ~task_manager();

    int current_id() const;
    void add_task(task_fn_t fn, const int32_t weight);
    void check_queue();
    void start_main_loop();

private:
    asio::io_context::strand& main_strand_;
    asio::io_context& pool_io_;
    int32_t concurrency_;
    int32_t aging_factor_;
    queue_t queue_;

    asio::steady_timer timer_;
    asio::steady_timer aging_timer_;
    std::atomic<int32_t> current_id_;
    std::atomic<int32_t> running_task_count_;
};

}  // namespace PG
