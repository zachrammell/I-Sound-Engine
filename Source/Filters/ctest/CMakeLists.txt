cmake_minimum_required(VERSION 3.16)

add_executable(Wav_Test_1 TestWav.cpp)

target_link_libraries(Wav_Test_1 opus)

add_test(WavContainerTest Wav_Test_1)

