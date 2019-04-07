# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(Multicore)

set(CMAKE_CXX_STANDARD 14)
#set(MY_STATIC_LINK true)
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS ON)  # only find static libs
    #set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
    set(Boost_USE_RELEASE_LIBS ON)  # only find release libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME ON)
endif ()

#find_package(OpenMP REQUIRED)

if (OPENMP_FOUND)
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -mavx2 -mavx -Ofast -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1")

#set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fstandalone-debug")
#set(CMAKE_VERBOSE_MAKEFILE TRUE)
#set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=undefined,integer,address -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=undefined,address -fno-omit-frame-pointer")

add_executable(${PROJECT_NAME}_hw1_cuda hw1_cuda.cpp)
add_executable(${PROJECT_NAME}_hw1_omp hw1_omp.cpp)

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_hw1_cuda PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_hw1_omp PROPERTIES LINK_SEARCH_START_STATIC 1)

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif ()
