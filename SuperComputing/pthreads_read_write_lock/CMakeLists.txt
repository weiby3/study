cmake_minimum_required(VERSION 3.5)
project(pthreads_read_write_lock)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} pthreads_link_list_read_write_lock.cpp my_rand.h timer.h my_read_write_lock.h)

target_link_libraries(${PROJECT_NAME} Threads::Threads)
