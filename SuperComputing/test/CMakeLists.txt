cmake_minimum_required(VERSION 3.5)
project(test)

set(CMAKE_CXX_STANDARD 11)
set(MY_STATIC_LINK true)
#set(BOOST_LIBRARYDIR "C:\\msys64\\mingw32\\lib\\")
set(Boost_DEBUG 1)
set(_boost_COMPILER "")
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS       ON)  # only find static libs
    set(Boost_USE_DEBUG_LIBS        ON) # ignore debug libs and
    set(Boost_USE_RELEASE_LIBS      OFF)  # only find release libs
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    ON)
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(${PROJECT_NAME}_test test.cpp)
add_executable(${PROJECT_NAME}_test2 test2.cpp)
add_executable(${PROJECT_NAME}_gfl_chip gfl_chip.cpp)

target_link_libraries(${PROJECT_NAME}_test2 ${Boost_LIBRARIES} wsock32 ws2_32)
target_link_libraries(${PROJECT_NAME}_gfl_chip ${Boost_LIBRARIES} wsock32 ws2_32)
#link_libraries(ws2_32)

include_directories(${PROJECT_NAME}_test ${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_NAME}_test2 ${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_NAME}_gfl_chip ${Boost_INCLUDE_DIRS})

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_test PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_test PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_test2 PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_test2 PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_gfl_chip PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_gfl_chip PROPERTIES LINK_SEARCH_END_STATIC 1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
