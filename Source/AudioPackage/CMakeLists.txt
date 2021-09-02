cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory(ctest)

# PackageEncoder Target
set(ENCODER_FILES PackageEncoder.cpp ../Containers/WavFile.h)
add_library(PackageEncoder ${ENCODER_FILES})
target_link_libraries(PackageEncoder opus AudioFiles)

