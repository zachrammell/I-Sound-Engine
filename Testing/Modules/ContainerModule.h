//
// Created by zack on 9/26/21.
//

#ifndef I_SOUND_ENGINE_CONTAINERMODULE_H
#define I_SOUND_ENGINE_CONTAINERMODULE_H

#include <OpusEncoderWrapper.h>
#include "gtest/gtest.h"
#include "WavFile.h"
#include "benchmark/benchmark.h"

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
    //readWaveFile("TestFiles/24_bit_reaper.wav");
}

TEST(WavFiles, Complex_32_bit_read)
{
    //readWaveFile("TestFiles/32_bit_reaper.wav");
}

#include "OpusFile.h"
#include "OpusDecoderWrapper.h"

TEST(OpusFiles, OpusReadFile)
{
    WavFile file("TestFiles/16_bit_reaper.wav");
    OpusEncoderWrapper encoder(48000, file.GetFormat().channel_count);
    OpusDecoderWrapper decoder(48000, file.GetFormat().channel_count);

    char* data = new char[file.GetDataSize() * 2];
    file.GetDataInNativeType(reinterpret_cast<char*>(data));

    unsigned char* opusData = new unsigned char[file.GetDataSize()];

    int readIndex = 0;
    int writeIndex = 0;

    while(readIndex < file.GetDataSize() / 2)
    {
        int value = encoder.Encode(reinterpret_cast<short *>(data) + readIndex, 960,
                                   reinterpret_cast<char *>(opusData + writeIndex + sizeof(int)) , file.GetDataSize());
        ASSERT_TRUE(value > 0);
        *reinterpret_cast<int*>(opusData + writeIndex) = value;
        writeIndex += value + sizeof(int);
        readIndex += 960;
    }

    //OpusDecoderWrapper decoder(48000, 1);
    int decodeIndex = 0;
    int decodeWrite = 0;
//    while(decodeIndex < writeIndex)
//    {
//        //int value = decoder(opusData, 960, )
//    }




    int readCount = 0;
    int totalRead = 0;
    while(readCount < writeIndex)
    {
        int frameSize = *reinterpret_cast<int*>(opusData + readCount);
        int returnValue = decoder.Decode(reinterpret_cast<char *>(opusData + readCount + sizeof(int)), 960, reinterpret_cast<short *>(data), 960);
        totalRead += returnValue;
        ASSERT_TRUE(returnValue >= 0);
        readCount += frameSize + sizeof(int);
    }

    std::fstream tesConvert("TestFiles/ConvertedOpus.wav", std::ios_base::binary | std::ios_base::out);
    RiffHeader riffHeader{{'R','I','F','F'},
                          0,
                          {'W','A','V','E'}};
    tesConvert.write(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));
    tesConvert.write(reinterpret_cast<const char*>(&file.GetFormat()), sizeof(FormatHeader));
    GenericHeaderChunk dataChunk{{'d', 'a','t','a'}, 0};
    dataChunk.chunkSize = file.GetDataSize();
    tesConvert.write(reinterpret_cast<char*>(&dataChunk), sizeof(dataChunk));
    tesConvert.write(data, file.GetDataSize());
    //OpusFile file("TestFiles/16_bit_reaper.opus");
}

static void Read100WavFilesExpected(benchmark::State& state)
{
    // Not timed
    for (auto _ : state)
    {
        char* bufferOfBuffers[100];
        for(int i = 0; i < 100; ++i)
        {
            WavFile wavFile("TestFiles/Slash2.wav");
            bufferOfBuffers[i] = new char[(wavFile.GetDataSize() / 2) * sizeof(float)];
            wavFile.GetDataAsFloat(reinterpret_cast<float*>(bufferOfBuffers[i]));
        }
        for(int i = 0; i < 100; ++i)
        {
            delete [] bufferOfBuffers[i];
        }
    }

}
BENCHMARK(Read100WavFilesExpected);

static void Read100WavFilesBrutal(benchmark::State& state)
{
    // Not timed
    for (auto _ : state)
    {
        char* bufferOfBuffers[100];
        for(int i = 0; i < 100; ++i)
        {
            WavFile wavFile("TestFiles/16_bit_reaper.wav");
            bufferOfBuffers[i] = new char[(wavFile.GetDataSize() / 2) * sizeof(float)];
            wavFile.GetDataAsFloat(reinterpret_cast<float*>(bufferOfBuffers[i]));
        }
        for(int i = 0; i < 100; ++i)
        {
            delete [] bufferOfBuffers[i];
        }
    }

}
BENCHMARK(Read100WavFilesBrutal);
