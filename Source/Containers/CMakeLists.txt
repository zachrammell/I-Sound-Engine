cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#add_subdirectory(ctest)

# PackageEncoder Target
set(AUDIO_FILES WavFile.h WavFile.cpp WavHeader.h)
add_library(AudioFiles ${AUDIO_FILES})
target_link_libraries(AudioFiles opus)

