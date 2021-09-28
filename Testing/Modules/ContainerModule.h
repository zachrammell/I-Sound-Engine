//
// Created by zack on 9/26/21.
//

#ifndef I_SOUND_ENGINE_CONTAINERMODULE_H
#define I_SOUND_ENGINE_CONTAINERMODULE_H

#include "gtest/gtest.h"

void readWaveFile(std::string name);

#endif //I_SOUND_ENGINE_CONTAINERMODULE_H


TEST(WavFiles, Simple_8_bit_read)
{
    readWaveFile("TestFiles/8_bit_simple.wav");

}
TEST(WavFiles, Complex_8_bit_read)
{
    readWaveFile("TestFiles/8_bit_reaper.wav");
}

TEST(WavFiles, Simple_16_bit_read)
{
    readWaveFile("TestFiles/16_bit_simple.wav");

}
TEST(WavFiles, Complex_16_bit_read)
{
    readWaveFile("TestFiles/16_bit_reaper.wav");
}

TEST(WavFiles, Complex_24_bit_read)
{
    readWaveFile("TestFiles/24_bit_reaper.wav");
}

TEST(WavFiles, Complex_32_bit_read)
{
    readWaveFile("TestFiles/32_bit_reaper.wav");
}