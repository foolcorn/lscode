cmake_minimum_required(VERSION 3.16)
project(lscode)
find_package(Threads)
set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(posix)

<<<<<<< HEAD
add_executable(lscode posix/utils.h MemoryPool.cpp LockFreeQueue.cpp)
=======
add_executable(lscode posix/utils.h LockFreeQueue.cpp)
>>>>>>> bea49a95b74a504d965db454435bc1277334674c
TARGET_LINK_LIBRARIES(lscode ${CMAKE_THREAD_LIBS_INIT})
