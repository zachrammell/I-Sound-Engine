cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory(ctest)

add_library(Containers INTERFACE)

target_include_directories(Containers INTERFACE)
