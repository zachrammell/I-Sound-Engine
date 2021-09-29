cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory(ctest)

#add_library(Parsers INTERFACE ${CMAKE_SOURCE_DIR})

#target_include_directories(Parsers INTERFACE ${CMAKE_SOURCE_DIR})

set(HEADER_FILES SoundContainer.h WavContainer.h ../Parsers/WavHeader.h)

add_library(Container INTERFACE ${HEADER_FILES})
target_include_directories(Container PRIVATE)

