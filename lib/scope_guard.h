// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#pragma once

#include <functional>

namespace PG {

class scope_guard {
public:
    explicit scope_guard(std::function<void()> fn)
        : fn_(std::move(fn))
    {}

    ~scope_guard() { fn_(); }

private:
    std::function<void()> fn_;
};

}  // namespace PG
