//
// Created by zack on 9/28/21.
//

#ifndef I_SOUND_ENGINE_PACKAGEMODULE_H
#define I_SOUND_ENGINE_PACKAGEMODULE_H

#include "gtest/gtest.h"

#include "PackageEncoder.h"
#include "PackageDecoder.h"
#include "WavFile.h"
#include "benchmark/benchmark.h"

void addFile(std::vector<WavFile>&)
{

}

template<typename... T>
void addFile(std::vector<WavFile> &vec, std::string fileName, T... files)
{
    vec.emplace_back(WavFile{fileName});
    ASSERT_TRUE(vec.back());
    addFile(vec, files...);
}

void Compare( std::unordered_map<uint64_t, SoundData>&, int)
{

}

template<typename... T>
void Compare( std::unordered_map<uint64_t, SoundData>& ParsedData, int index, std::string fileName, T... rest)
{
    WavFile wavFile(fileName);
    ASSERT_TRUE(wavFile);

    char* data = new char[wavFile.GetDataSize() * 4];

    wavFile.GetDataAsFloat(reinterpret_cast<float *>(data));

    for(int i = 0; i < ParsedData[index].sampleCount; ++i)
    {
        float left = *reinterpret_cast<float*>(ParsedData[index].data + (i * sizeof(float)));
        float right = *reinterpret_cast<float*>(data + (i * sizeof(float)));
        ASSERT_FLOAT_EQ(left, right);
    }

    delete [] data;

    Compare(ParsedData, ++index, rest...);
}

template<typename... T>
void TestPackages(std::string outName, T... toRead)
{
    //--------------------------------------------------
    // ENCODING
    //--------------------------------------------------
    {
    std::vector<WavFile> files;

    addFile(files, toRead...);

    PackageEncoder encoder;

    for (int i = 0; i < files.size(); ++i)
    {
        encoder.AddFile(files[i], i, Encoding::PCM);
    }

    ASSERT_TRUE(encoder.WritePackage(outName) == ErrorNum::NoErrors);
    }

    //--------------------------------------------------
    // DECODING
    //--------------------------------------------------

    std::unordered_map<uint64_t, SoundData> ParsedData;
    char* dataPointer;
    PackageDecoder::DecodePackage(ParsedData, &dataPointer, outName);

    //delete [] dataPointer;

    Compare(ParsedData, 0, toRead...);

}



TEST(Package, EncodeSimpleWav)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_simple.wav");
}

TEST(Package, EncodeComplexWav)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_reaper.wav");
}

TEST(Package, Encode2SimpleWav)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_simple.wav", "TestFiles/16_bit_simple.wav");
}

TEST(Package, Encode2ComplexWav)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_reaper.wav", "TestFiles/16_bit_reaper.wav");
}

TEST(Package, Encode2DifferntWav)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_simple.wav", "TestFiles/16_bit_reaper.wav");
}

TEST(Package, Encode2DiffertBitRate)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/8_bit_simple.wav", "TestFiles/16_bit_simple.wav");
}

TEST(Package, Encode10)
{
    TestPackages("TestFiles/TESTBank1Wav.pak", "TestFiles/16_bit_reaper.wav",
                 "TestFiles/16_bit_reaper.wav", "TestFiles/16_bit_reaper.wav",
                 "TestFiles/16_bit_reaper.wav", "TestFiles/16_bit_reaper.wav",
                 "TestFiles/16_bit_reaper.wav", "TestFiles/16_bit_reaper.wav",
                 "TestFiles/16_bit_reaper.wav", "TestFiles/16_bit_reaper.wav",
                 "TestFiles/16_bit_reaper.wav");
}
#endif //I_SOUND_ENGINE_PACKAGEMODULE_H

static void Encode100WavExpectedFilePack(benchmark::State& state)
{
    for(auto _ : state)
    {
        PackageEncoder encoder;
        WavFile wav("TestFiles/Slash2.wav");
        for(int i = 0; i < 100; ++i)
        {
            encoder.AddFile(wav, i, PCM);
        }
        encoder.WritePackage("TestFiles/TEST100WavFilesExpected.pak");
    }
}
BENCHMARK(Encode100WavExpectedFilePack);

static void Encode100WavBurtalFilePack(benchmark::State& state)
{
    for(auto _ : state)
    {
        PackageEncoder encoder;
        WavFile wav("TestFiles/16_bit_reaper.wav");
        for(int i = 0; i < 100; ++i)
        {
            encoder.AddFile(wav, i, PCM);
        }
        encoder.WritePackage("TestFiles/TEST100WavFilesBurtal.pak");}
}
BENCHMARK(Encode100WavBurtalFilePack);

static void Read1_100FilePackExpected(benchmark::State& state)
{
    PackageEncoder encoder;
    WavFile wav("TestFiles/Slash2.wav");
    for(int i = 0; i < 100; ++i)
    {
        encoder.AddFile(wav, i, PCM);
    }
    encoder.WritePackage("TestFiles/TEST100WavFilesExpected.pak");

    for (auto _ : state)
    {
        std::unordered_map<uint64_t, SoundData> ParsedData;
        char* dataPointer;
        PackageDecoder::DecodePackage(ParsedData, &dataPointer, "TestFiles/TEST100WavFilesExpected.pak");
        //delete [] dataPointer;
    }
}
BENCHMARK(Read1_100FilePackExpected);

static void Read1_100FilePackBrutal(benchmark::State& state)
{
    PackageEncoder encoder;
    WavFile wav("TestFiles/16_bit_reaper.wav");
    for(int i = 0; i < 100; ++i)
    {
        encoder.AddFile(wav, i, PCM);
    }
    encoder.WritePackage("TestFiles/TEST100WavFilesBurtal.pak");

    for (auto _ : state)
    {
        std::unordered_map<uint64_t, SoundData> ParsedData;
        char* dataPointer;
        PackageDecoder::DecodePackage(ParsedData, &dataPointer, "TestFiles/TEST100WavFilesBurtal.pak");
        //delete [] dataPointer;
    }
}
BENCHMARK(Read1_100FilePackBrutal);

static void EncodeLevel44100ToOpus(benchmark::State& state)
{
    WavFile file("TestFiles/level.wav");
    char *buffer = new char[file.GetDataSize()];
    for (auto _: state)
    {
        file.GetDataAsOpus(buffer);
    }
}
BENCHMARK(EncodeLevel44100ToOpus);

static void EncodeCredits48000ToOpus(benchmark::State& state)
{
    WavFile file("TestFiles/credits.wav");
    char *buffer = new char[file.GetDataSize()];
    for (auto _: state)
    {
        file.GetDataAsOpus(buffer);
    }
}
BENCHMARK(EncodeCredits48000ToOpus);