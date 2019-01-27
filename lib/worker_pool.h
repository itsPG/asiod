// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <atomic>
#include <cstdint>
#include <thread>

#include <boost/asio/executor_work_guard.hpp>

#include "task_manager.h"
#include "util.h"

namespace PG {

namespace asio = boost::asio;

class worker_pool {
public:
    explicit worker_pool(asio::io_context::strand&, int32_t concurrency);
    ~worker_pool();

    int32_t add_task(task_fn_t fn, const int32_t weight);
    void check_queue();

private:
    asio::io_context::strand main_strand_;
    int32_t concurrency_;

    asio::io_context pool_io_;
    asio::executor_work_guard<asio::io_context::executor_type> work_;
    std::atomic<bool> is_stopped_;
    vector<std::thread> thread_pool_;
    task_manager task_manager_;
};

}  // namespace PG
