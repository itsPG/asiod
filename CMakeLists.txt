# by PG, MIT license.
# this repo is for practicing boost::asio, not well tested, use it at your own risk.

cmake_minimum_required (VERSION 3.10)
project (asio)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS
	coroutine
	filesystem
	system
)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(lib)

# server binary
add_executable(server
	server.cpp
)
target_link_libraries(server PRIVATE
	asio
)

# client binary
add_executable(client
    client.cpp
)
target_link_libraries(client PRIVATE
	asio
)
