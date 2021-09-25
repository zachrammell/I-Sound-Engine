
add_executable(EncoderTest testEncode.cpp)

target_compile_options(EncoderTest PRIVATE -g)

target_link_libraries(EncoderTest PackageEncoder)

add_test(WavSingleEncode EncoderTest ${CMAKE_CURRENT_SOURCE_DIR}/testPack.pak ${CMAKE_CURRENT_SOURCE_DIR}/8_bit_simple.wav)