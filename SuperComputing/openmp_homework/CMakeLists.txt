cmake_minimum_required(VERSION 3.10)
project(openmp_homework)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME}_producer_consumer producer_consumer.c)
add_executable(${PROJECT_NAME}_multi_producer_multi_consumer multi_producer_multi_consumer.cpp)

target_link_libraries(${PROJECT_NAME}_producer_consumer OpenMP::OpenMP_C)
target_link_libraries(${PROJECT_NAME}_multi_producer_multi_consumer OpenMP::OpenMP_CXX)
