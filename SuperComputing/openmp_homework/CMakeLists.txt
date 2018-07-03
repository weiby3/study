cmake_minimum_required(VERSION 3.5)
project(openmp_homework)

set(CMAKE_CXX_STANDARD 11)
#set(MY_STATIC_LINK true)
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS       ON)  # only find static libs
    set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
    set(Boost_USE_RELEASE_LIBS      ON)  # only find release libs
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    ON)
endif()

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(${PROJECT_NAME}_producer_consumer producer_consumer.c)
add_executable(${PROJECT_NAME}_multi_producer_multi_consumer multi_producer_multi_consumer.cpp)
add_executable(${PROJECT_NAME}_dijkstra dijkstra.cpp)

#target_link_libraries(${PROJECT_NAME}_producer_consumer OpenMP::OpenMP_C)
#target_link_libraries(${PROJECT_NAME}_multi_producer_multi_consumer OpenMP::OpenMP_CXX)

include_directories(${PROJECT_NAME}_multi_producer_multi_consumer ${Boost_INCLUDE_DIRS})

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_multi_producer_multi_consumer PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_multi_producer_multi_consumer PROPERTIES LINK_SEARCH_END_STATIC 1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
