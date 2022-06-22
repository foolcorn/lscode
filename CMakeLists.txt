cmake_minimum_required(VERSION 3.20)
project(lscode)

set(CMAKE_CXX_STANDARD 11)

add_executable(lscode BTree.cpp design-pattern/singleton.cpp)
