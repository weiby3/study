# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(ArtificialIntelligent)

set(CMAKE_CXX_STANDARD 11)
set(Boost_DEBUG 1)
#set(MY_STATIC_LINK true)
if (MY_STATIC_LINK)
    set(Boost_USE_STATIC_LIBS       ON)  # only find static libs
    #set(Boost_USE_DEBUG_LIBS        OFF) # ignore debug libs and
    #set(Boost_USE_RELEASE_LIBS      ON)  # only find release libs
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    ON)
    #set(BOOST_LIBRARYDIR C:/msys64/mingw32/lib)
endif()



#set(CMAKE_TOOLCHAIN_FILE C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake)
#find_package(Eigen3 CONFIG REQUIRED)
find_package(Boost COMPONENTS
        program_options
        REQUIRED)

add_executable(${PROJECT_NAME}_ex1_tfidf tfidf.cpp tf.h idf.h)
add_executable(${PROJECT_NAME}_ex1_knn knn.cpp one_hot.h)
target_link_libraries(${PROJECT_NAME}_ex1_tfidf PRIVATE ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_ex1_knn PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})

if (MY_STATIC_LINK)
    set_target_properties(${PROJECT_NAME}_ex1_tfidf PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_tfidf PROPERTIES LINK_SEARCH_END_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_knn PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(${PROJECT_NAME}_ex1_knn PROPERTIES LINK_SEARCH_END_STATIC 1)

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
