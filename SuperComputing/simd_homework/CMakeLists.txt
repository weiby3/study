cmake_minimum_required(VERSION 3.10)
project(simd_homework)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2 -mavx -mavx2 -Ofast")

add_executable(${PROJECT_NAME} canny_edge.c)
add_executable(${PROJECT_NAME}_test_int simdsumint.c)
add_executable(${PROJECT_NAME}_test_double simdsumdouble.c)