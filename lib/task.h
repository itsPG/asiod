// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

// asio only supports coroutine, not coroutine2
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING
// ensure we're using the current api version
#define BOOST_ASIO_NO_DEPRECATED

#include <functional>

#include "util.h"

namespace PG {

// TODO: It seens like asio only accept std::function<void()> ? (I'm not sure)
using task_fn_t = std::function<void()>;

struct task {
    int32_t id;
    int32_t weight;
    task_fn_t fn;
};

}  // namespace PG
