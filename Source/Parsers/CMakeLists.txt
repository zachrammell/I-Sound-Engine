cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# PackageEncoder Target
set(AUDIO_FILES WavFile.h WavFile.cpp WavHeader.h OpusFile.cpp OpusFile.h OpusDecoderWrapper.cpp OpusDecoderWrapper.h OpusHeader.h)
add_library(AudioFiles ${AUDIO_FILES})
#target_link_libraries(AudioFiles opus)
target_link_libraries(AudioFiles opus)
target_include_directories(AudioFiles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
