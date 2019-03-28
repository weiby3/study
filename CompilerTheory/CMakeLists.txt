# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(CompilerTheory)

set(CMAKE_CXX_STANDARD 14)
#set(MY_STATIC_LINK true)
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS ON)  # only find static libs
    #set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
    #set(Boost_USE_RELEASE_LIBS      ON)  # only find release libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME ON)
endif ()

#set(CMAKE_CXX_FLAGS_DEBUG "-v -g3 -fstandalone-debug")
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=undefined,integer,address -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=undefined,address -fno-omit-frame-pointer")

add_executable(${PROJECT_NAME}_04_02 04_02.cpp)

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_04_02 PROPERTIES LINK_SEARCH_START_STATIC 1)

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif ()
