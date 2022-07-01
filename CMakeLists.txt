cmake_minimum_required(VERSION 3.16)
project(lscode)
find_package(Threads)
set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(posix)

add_executable(lscode posix/utils.h posix/epoll_v1.cpp)
TARGET_LINK_LIBRARIES(lscode ${CMAKE_THREAD_LIBS_INIT})
