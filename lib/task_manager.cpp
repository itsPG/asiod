// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include "task_manager.h"

#include <boost/asio/dispatch.hpp>

#include "packet.h"
#include "scope_guard.h"
#include "task.h"
#include "util.h"

namespace PG {

bool task_comparator(const task& lhs, const task& rhs)
{
    if (lhs.weight != rhs.weight) {
        return lhs.weight < rhs.weight;
    } else {
        return lhs.id > rhs.id;
    }
}

task_manager::task_manager(asio::io_context::strand& main_strand, asio::io_context& pool_io, int32_t concurrency)
    : main_strand_{main_strand}
    , pool_io_{pool_io}
    , concurrency_{concurrency}
    , aging_factor_{0}
    , queue_{task_comparator}
    , timer_{main_strand_.context()}
    , aging_timer_{main_strand_.context()}
    , current_id_{0}
    , running_task_count_{0}
{}

task_manager::~task_manager() {}

void task_manager::add_task(task_fn_t fn, const int32_t weight)
{
    task t;
    t.id = current_id_.fetch_add(1);
    t.weight = weight + aging_factor_;
    t.fn = [fn = std::move(fn), this]() {
        scope_guard guard([&]() {
            running_task_count_.fetch_sub(1);
            check_queue();
        });
        fn();
    };

    asio::dispatch(main_strand_, [t = std::move(t), this]() { queue_.emplace(std::move(t)); });
}

void task_manager::start_main_loop()
{
    cout << "task_manager::start_main_loop()" << endl;

    asio::spawn(main_strand_, [this](asio::yield_context yield) {
        while (true) {
            // dispatch task from queue to worker pool
            while (!queue_.empty() && running_task_count_.load() < concurrency_) {
                running_task_count_.fetch_add(1);
                asio::post(pool_io_, queue_.top().fn);
                queue_.pop();
            }

            cout << "running_task_count_ " << running_task_count_.load() << " waiting.." << endl;

            // wait for next notify
            timer_.expires_at(std::chrono::time_point<std::chrono::steady_clock>::max());
            boost::system::error_code ec;
            timer_.async_wait(yield[ec]);
        }
    });

    asio::spawn(main_strand_, [this](asio::yield_context yield) {
        while (true) {
            aging_timer_.expires_after(std::chrono::seconds(1));
            boost::system::error_code ec;
            aging_timer_.async_wait(yield[ec]);
            --aging_factor_;
        }
    });
}

void task_manager::check_queue()
{
    // cancel the timer to notify the main_loop to dispatch the next task
    asio::dispatch(main_strand_, [this]() { timer_.cancel(); });
}

int32_t task_manager::current_id() const
{
    return current_id_.load();
}

}  // namespace PG
