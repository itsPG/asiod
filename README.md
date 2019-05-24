# Introduction

It's hard to find boost::asio v1.68 working examples (at this time).

So I create one.

This simple project demostracte how to use boost::asio to build a daemon

In this example, server receives packet from client via doamin socket, and then dispatch task to worker_pool. worker_pool holds threads and those threads process request by pool_io, which is an io_context instance for dispatching tasks among std::threads.

NOTE: it's not well tested yet, use it at your own risk.

# Dependency

This project is written and tested in macOS.

You'll need boost, cmake, clang-format.

Those dependency can be simply installed by

```
brew install cmake
brew install boost
brew install clang-format
```

# Build

```
git clone https://github.com/itsPG/asiod.git
cd asiod
./build.sh
```

# Run

### Server

```
cd build
./server
```

### Client

```
cd build
./client
```

# TODO & issues

- Due to there's no (suitable) way to stop a thread, we should remove the 30s timeout code, it's useless
- Refine session, process_command is no needed to be a virtual function (combination is better than inherient)
- No need to add "worker_pool" layer, try to integrate it with task_manager
- Sesson may need no "enable_share_from_this" (TBD)

# Reference

There're some good websites help me write this project.

The Boost Cpp Libraries
https://theboostcpplibraries.com/
https://github.com/BorisSchaeling/theboostcpplibraries

Asio C++ Library
https://think-async.com/Asio/

基于 Asio 的 C++ 网络编程
https://segmentfault.com/a/1190000007225464

Boost.Asio的使用技巧
http://blog.jqian.net/post/boost-asio.html

浅谈 Boost.Asio 的多线程模型
http://senlinzhan.github.io/2017/09/17/boost-asio/


# License

MIT
