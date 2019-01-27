// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "worker_pool.h"

#include "packet.h"
#include "scope_guard.h"
#include "util.h"

namespace PG {

worker_pool::worker_pool(asio::io_context::strand& main_strand, int32_t concurrency)
    : main_strand_{main_strand}
    , concurrency_{concurrency}
    , pool_io_{}
    , work_{asio::make_work_guard(pool_io_)}
    , is_stopped_{false}
    , thread_pool_{}
    , task_manager_{main_strand_, pool_io_, concurrency_}
{
    auto worker = [this]() {
        while (!is_stopped_.load()) {
            try {
                pool_io_.run();
            } catch (const std::runtime_error& e) {
                cout << "thread std::runtime_error " << e.what() << endl;
            } catch (const std::exception& e) {
                cout << "thread std::exception " << e.what() << endl;
            }
        }
    };

    for (int32_t i = 0; i < concurrency_; ++i) {
        thread_pool_.emplace_back(worker);
    }

    task_manager_.start_main_loop();
}

worker_pool::~worker_pool()
{
    is_stopped_.store(true);
    pool_io_.stop();

    for (auto& thread : thread_pool_) {
        thread.join();
    }
}

int32_t worker_pool::add_task(task_fn_t fn, const int32_t weight)
{
    task_manager_.add_task(std::move(fn), weight);
    return task_manager_.current_id();
}

void worker_pool::check_queue()
{
    task_manager_.check_queue();
}

}  // namespace PG
