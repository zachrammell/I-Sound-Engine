cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# PackageEncoder Target
set(ENCODER_FILES PackageEncoder.cpp ../Parsers/WavFile.h)
set(DECODER_FILES PackageDecoder.cpp ../Parsers/WavFile.h)

add_library(PackageDecoder ${DECODER_FILES})
add_library(PackageEncoder ${ENCODER_FILES})
target_link_libraries(PackageEncoder #[[opus]] AudioFiles)

target_include_directories(PackageEncoder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(PackageDecoder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
