# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(ArtificialIntelligent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
#set(Boost_DEBUG 1)
#set(HOME_UBUNTU 1)
set(BHPC_SERVER 1)
if (HOME_UBUNTU)
    set(BOOST_ROOT /var/boost/installed)
endif ()
if (BHPC_SERVER)
    set(BOOST_ROOT ~/var/boost/installed)
endif ()
#set(MY_STATIC_LINK true)
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS ON)  # only find static libs
    #set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
    #set(Boost_USE_RELEASE_LIBS      ON)  # only find release libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    #set(BOOST_LIBRARYDIR C:/msys64/mingw32/lib)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-v -g3 -fstandalone-debug")

#set(CMAKE_TOOLCHAIN_FILE C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake)
#find_package(Eigen3 CONFIG REQUIRED)
find_package(Boost COMPONENTS
        program_options
        mpi
        REQUIRED)
find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME}_ex1_tfidf tfidf.cpp tf.h idf.h)
add_executable(${PROJECT_NAME}_ex1_knn knn.cpp one_hot.h)
add_executable(${PROJECT_NAME}_ex6_reversi reversi.cpp)
add_executable(${PROJECT_NAME}_ex7_n_queen n_queen.cpp)
target_link_libraries(${PROJECT_NAME}_ex1_tfidf PRIVATE ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_ex1_knn PRIVATE ${Boost_LIBRARIES})
#target_link_libraries(${PROJECT_NAME}_ex6_reversi PRIVATE ${Boost_LIBRARIES} ${MPI_CXX_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_DIRS})

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_ex1_tfidf PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_tfidf PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_knn PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_knn PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex6_reversi PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex6_reversi PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex7_n_queen PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex7_n_queen PROPERTIES LINK_SEARCH_END_STATIC 1)

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif ()
