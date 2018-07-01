cmake_minimum_required(VERSION 3.10)
project(openmp_homework)

set(CMAKE_CXX_STANDARD 11)
set(Boost_USE_STATIC_LIBS       ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS      ON)  # only find release libs
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    ON)

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME}_producer_consumer producer_consumer.c)
add_executable(${PROJECT_NAME}_multi_producer_multi_consumer multi_producer_multi_consumer.cpp)

target_link_libraries(${PROJECT_NAME}_producer_consumer OpenMP::OpenMP_C)
target_link_libraries(${PROJECT_NAME}_multi_producer_multi_consumer OpenMP::OpenMP_CXX)

set_target_properties(${PROJECT_NAME}_multi_producer_multi_consumer PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(${PROJECT_NAME}_multi_producer_multi_consumer PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
