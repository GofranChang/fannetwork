cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)

project(fannetwork)

set(TASK_SRC
  src/reactor.h
  src/reactor.cpp
  src/socket.h
  src/socket.cpp
  src/task_scheduler.h
  src/task_scheduler.cpp
  src/tcp_server.cpp
)

add_subdirectory(third_party/libevent libevent.out EXCLUDE_FROM_ALL)
include_directories(third_party/libevent/include)
include_directories(${PROJECT_BINARY_DIR}/libevent.out/include)

include_directories(src)
include_directories(src/common)
include_directories(interface)

add_library(fannetwork SHARED
    ${NETWORK_SRC}
    ${TASK_SRC})

target_link_libraries(fannetwork event)

add_executable(echo_server example/echo_server.cpp)
target_link_libraries(echo_server fannetwork)

add_executable(client example/client.cpp)
target_link_libraries(client fannetwork)
