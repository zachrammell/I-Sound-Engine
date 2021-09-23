cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


add_executable(WavOpen WavTest.cpp)

target_link_libraries(WavOpen AudioFiles)

add_test(Wac_Open_8_bits_Simple WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/8_bit_simple.wav)
add_test(Wac_Open_16_bits_Simple WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/16_bit_simple.wav)
add_test(Wav_Open_8_bits_Reaper WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/8_bit_reaper.wav)
add_test(Wav_Open_16_bits_Reaper WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/16_bit_reaper.wav)
add_test(Wav_Open_24_bits_Reaper WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/24_bit_reaper.wav)
add_test(Wav_Open_32_bits_Reaper WavOpen ${CMAKE_CURRENT_SOURCE_DIR}/32_bit_reaper.wav)



#message(${CMAKE_SOURCE_DIR})
#target_link_libraries(WavOpen ${CMAKE_SOURCE_DIR}/opus.lib)
#target_link_libraries(WavOpen opus)