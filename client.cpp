// by PG, MIT license.
// this repo is for practicing boost::asio, not well tested, use it at your own risk.

#include <thread>

#include "socket_client.h"

constexpr auto domain_socket_path = "/tmp/PG_asio.sock";

int main()
{
    using namespace PG;

    boost::asio::io_context io_context;
    boost::asio::io_context::strand strand{io_context};
    socket_client c(strand, domain_socket_path);

    while (true) {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            string req{"qwerqwer"};
            string resp{c.send_and_recv(req)};

            io_context.run();
            io_context.restart();
        } catch (const std::runtime_error& e) {
            cout << "[Client std::runtime_error] " << e.what() << endl;
            break;
        }
    }
}