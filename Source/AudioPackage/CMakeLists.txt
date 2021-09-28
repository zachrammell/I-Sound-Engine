cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# PackageEncoder Target
set(ENCODER_FILES PackageEncoder.cpp ../Containers/WavFile.h)
set(DECODER_FILES PackageDecoder.h PackageDecoder.cpp)

add_library(PackageDecoder INTERFACE ${DECODER_FILES})
add_library(PackageEncoder ${ENCODER_FILES})
target_link_libraries(PackageEncoder #[[opus]] AudioFiles)

