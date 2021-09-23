cmake_minimum_required(VERSION 3.20)
project(I_Sound_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS false)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

enable_testing()


add_subdirectory(Source)
add_subdirectory(Libraries)


#link_directories(Libraries/Opus)
#link_directories(Libraries/Opus/include)