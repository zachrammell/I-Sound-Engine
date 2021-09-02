cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#target_link_libraries(AudioFiles opus)

add_executable(WavOpen WavTest.cpp)

target_link_libraries(WavOpen AudioFiles)

add_test(Wav_Open_8_bits_Reaper WavOpen 8_bit_reaper.wav)
add_test(Wac_Open_16_bits_Reaper WavOpen 16_bit_reaper.wav)