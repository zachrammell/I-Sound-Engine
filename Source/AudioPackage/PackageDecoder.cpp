//
// Created by zack on 9/23/21.
//

#include <fstream>
#include "../Parsers/WavHeader.h"
#include "PackageDecoder.h"
#include <sys/mman.h>
#include "../Parsers/OpusHeader.h"
#include "../Parsers/OpusFile.h"

#include <fcntl.h>

constexpr int samplesPerPacket = 960;

namespace PackageDecoder
{
    int ParseWav(int id, std::unordered_map<uint64_t, SoundData>& table, char* data)
    {
        SoundData soundData;
        soundData.data = data + sizeof(WavHeader);
        soundData.audioType = Encoding::PCM;

        // Read the header
        // TODO Could optimize to only read needed bytes if parsing is to slow
        WavHeader header = *reinterpret_cast<WavHeader*>(data);
        int offset = sizeof(header);

        // Find the number of samples
        soundData.sampleCount = (header.dataHeader.chunkSize / (header.formatHeader.bits_per_sample / 8));
        soundData.channels = header.formatHeader.channel_count;
        soundData.sampleRate = header.formatHeader.sampling_rate;

        // TODO add caclulation for sample rate
        //soundData.sampleCount * sizeof(float);
        offset += header.dataHeader.chunkSize;

        table[id] = soundData;
        return offset;
    }

    int ParseOpus(int id, std::unordered_map<uint64_t, SoundData>& table, char* data)
    {
        OpusHeaderChunk header;
        header = *reinterpret_cast<OpusHeaderChunk*>(data);
        int offset = 19; // Magic opus header size as size of gives 20 due to padding

        SoundData soundData;
        soundData.data = data + offset;
        soundData.sampleCount = 0;
        soundData.audioType = Opus;
        soundData.channels = header.channels;
        soundData.sampleRate = header.sampleRate;

        while(*(data + offset + 0) == 'O' &&
              *(data + offset + 1) == 'g' &&
              *(data + offset + 2) == 'g' &&
              *(data + offset + 3) == 'S')
        {
            int packetSize;
            offset += OpusFile::GetSegementSize(data + offset, packetSize);
            offset += packetSize;
            soundData.sampleCount += samplesPerPacket;
        }
        table[id] = soundData;
        return offset;
    }

    int GetDataStartAndLength(std::unordered_map<uint64_t, SoundData>& table, char* data, int size)
    {
        int offset = 0;
        while(offset < size)
        {
            // Read the id
            uint32_t id = *reinterpret_cast<const uint32_t*>(data + offset);
            offset += sizeof(uint32_t);

            if(*(data + offset + 0) == 'R' &&
               *(data + offset + 1) == 'I' &&
               *(data + offset + 2) == 'F' &&
               *(data + offset + 3) == 'F')
            {
                offset += ParseWav(id, table, data + offset);
            }
            else if(*(data + offset + 0) == 'O' &&
                    *(data + offset + 1) == 'p' &&
                    *(data + offset + 2) == 'u' &&
                    *(data + offset + 3) == 's' &&
                    *(data + offset + 4) == 'H' &&
                    *(data + offset + 5) == 'e' &&
                    *(data + offset + 6) == 'a' &&
                    *(data + offset + 7) == 'd')
            {
                offset += ParseOpus(id, table, data + offset);
            }
        }
        return offset;
    }

    void Convert16Bit(int size, short* in, float* out)
    {
        for(int i = 0; i < size; ++i)
        {
            out[i] = in[i] / static_cast<float>(std::numeric_limits<short>::max());
        }
    }

    void Convert8Bit(int size, unsigned char* in, float* out)
    {
        for(int i = 0; i < size; ++i)
        {
            out[i] = static_cast<float>(in[i]) - 128.0f;
            out[i] /= 128.0f;
        }
    }

    void ConvertToFloat(std::unordered_map<uint64_t, SoundData>& table, char* outputBuffer)
    {
        uint32_t offset = 0;
        for(auto& wav : table )
        {
            if(wav.second.audioType == PCM)
            {
                WavHeader header = *reinterpret_cast<WavHeader*>(wav.second.data);
                if(header.formatHeader.bits_per_sample / 8 == 1)
                {
                    Convert8Bit(wav.second.sampleCount, reinterpret_cast<unsigned char*>(wav.second.data + sizeof(WavHeader)),
                                reinterpret_cast<float *>(outputBuffer + offset));
                }
                else if(header.formatHeader.bits_per_sample / 8 == 2)
                {
                    Convert16Bit(wav.second.sampleCount, reinterpret_cast<short *>(wav.second.data + sizeof(WavHeader)),
                                 reinterpret_cast<float *>(outputBuffer + offset));
                }
                wav.second.data = outputBuffer + offset;

                offset += wav.second.sampleCount * sizeof(float);
            }
        }
    }

    ErrorNum DecodePackage(std::unordered_map<uint64_t, SoundData>& lookUpTable, char** data, std::string path)
    {
        // Check if bank exists
        std::fstream file(path, std::ios_base::binary | std::ios_base::in);
        if(!file.is_open())
            return  ErrorNum::FailedToFindFile;

        // Read in the entire file in one read so all work is done from memory
        uint32_t fileSize;
        file.read(reinterpret_cast<char*>(&fileSize), sizeof(uint32_t));

        // reading
//        int bufferRead = open("read.dat", O_CREAT | O_RDWR,
//        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);

        int file_read = open(path.c_str(), O_RDWR, 0);
        // writing
        int bufferWrite = open("write.dat", O_CREAT | O_RDWR,
                              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

        //
        char *fileBuffer = reinterpret_cast<char*>(
                mmap(NULL, fileSize+4, PROT_READ | PROT_WRITE , MAP_SHARED, file_read, 0));
        //char* fileBuffer = new char[fileSize];
        //posix_fallocate(bufferRead, 0, fileSize);

        //file.read(fileBuffer, fileSize);

        // Find start point of all data files
        int processedSize = GetDataStartAndLength(lookUpTable, fileBuffer+4, fileSize);

        // Create buffer to store PCM as floats in continues memory
        //*data = new char[processedSize];

        //ConvertToFloat(lookUpTable, *data);

        //munmap(fileBuffer,fileSize);

        //delete [] fileBuffer;
        return ErrorNum::NoErrors;
    }
}