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
    // File to convert
    WavFile file("TestFiles/credits.wav");

    // Create encders and decoders
    OpusEncoderWrapper encoder(48000, file.GetFormat().channel_count);
    OpusDecoderWrapper decoder(48000, file.GetFormat().channel_count);
    OpusDecoderWrapper altDecoder(48000, file.GetFormat().channel_count);

    // Space to store pcm samples
    char* data = new char[file.GetDataSize() * 2];
    char* altData = new char[file.GetDataSize() * 2];

    file.GetDataInNativeType(reinterpret_cast<char*>(data));

    // opus data buffer
    unsigned char* opusData = new unsigned char[file.GetDataSize()];

    // Indexs into all of the arrays
    int readIndex = 0;
    int writeIndex = 0;
    int altWriteIndex = 0;
    while(readIndex < file.GetDataSize() / 2)
    {
        // Encode frame
        int value = encoder.Encode(reinterpret_cast<short *>(data) + readIndex, 960,
                                   reinterpret_cast<char *>(opusData + writeIndex + sizeof(int)) , file.GetDataSize());
        ASSERT_TRUE(value > 0) << "Error Value" << value;

        // Decode frame
        int decodeValue = altDecoder.Decode(reinterpret_cast<char *>(opusData + writeIndex + sizeof(int)), value,
                                            reinterpret_cast<short *>(altData + altWriteIndex), 960);
        ASSERT_TRUE(decodeValue > 0) << "Error Value" << decodeValue;

        // Update indexes
        altWriteIndex += decodeValue;

        *reinterpret_cast<int*>(opusData + writeIndex) = value;
        writeIndex += value + sizeof(int);
        readIndex += 960/2;


    }

    //OpusDecoderWrapper decoder(48000, 1);
    int decodeIndex = 0;
    int decodeWrite = 0;
//    while(decodeIndex < writeIndex)
//    {
//        //int value = decoder(opusData, 960, )
//    }

    // Clear data buffer so no old data can be in it
    for(int i = 0; i < file.GetDataSize(); ++i)
    {
        data[i] = 0;
    }

    // Decode the entire audio
    int readCount = 0;
    int totalRead = 0;
    while(readCount < writeIndex)
    {
        int frameSize = *reinterpret_cast<int*>(opusData + readCount);
        int returnValue = decoder.Decode(reinterpret_cast<char *>(opusData + readCount + sizeof(int)), frameSize, reinterpret_cast<short *>(data + totalRead), 960);
        totalRead += returnValue;
        ASSERT_TRUE(returnValue >= 0);
        readCount += frameSize + sizeof(int);
    }

    ASSERT_EQ(totalRead, readIndex * 2);
    ASSERT_EQ(totalRead, altWriteIndex);

    for(int i = 0; i <totalRead; ++i)
    {
        ASSERT_EQ(data[i], altData[i]);
    }

    // Write out as a file again
    std::fstream tesConvert("TestFiles/ConvertedOpus.wav", std::ios_base::binary | std::ios_base::out);
    RiffHeader riffHeader{{'R','I','F','F'},
                          0,
                          {'W','A','V','E'}};
    tesConvert.write(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));
    tesConvert.write(reinterpret_cast<const char*>(&file.GetFormat()), sizeof(FormatHeader));
    GenericHeaderChunk dataChunk{{'d', 'a','t','a'}, 0};
    dataChunk.chunkSize = file.GetDataSize();
    tesConvert.write(reinterpret_cast<char*>(&dataChunk), sizeof(dataChunk));
    tesConvert.write(data, 960 * 2);
    //OpusFile file("TestFiles/16_bit_reaper.opus");

    OpusHeaderChunk opusHeader = {'O','p','u','s','H','e','a','d' };
    opusHeader.version = 1;
    opusHeader.channels = 2;
    opusHeader.sampleRate = 48000;
    opusHeader.preSkip = 0;
    opusHeader.outputGain = 0;
    opusHeader.mapingFamily = 0;

    OpusFile::WriteToFile(reinterpret_cast<char *>(opusData), altWriteIndex, opusHeader, "TestFiles/myConverted.ogg");
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
