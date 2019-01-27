# by PG, MIT license.
# this repo is for practicing boost::asio, not well tested, use it at your own risk.


add_library(asio STATIC
	socket_server.cpp
	socket_client.cpp
	session.cpp
	packet.cpp
	task_manager.cpp
	worker_pool.cpp
)

target_link_options(asio PUBLIC
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_COROUTINE_LIBRARY}
)

