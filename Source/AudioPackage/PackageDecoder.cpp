//
// Created by zack on 9/23/21.
//

#include <fstream>
#include "../Parsers/WavHeader.h"
#include "PackageDecoder.h"

namespace PackageDecoder
{
    int GetDataStartAndLength(std::unordered_map<uint64_t, SoundData>& table, char* data, int size)
    {
        int dataLength = 0;
        for(int offset = 0; offset < size;)
        {
            // Read the id
            uint32_t id = *reinterpret_cast<uint32_t*>(data + offset);
            offset += sizeof(uint32_t);

            SoundData soundData;
            soundData.data = data + offset;
            soundData.audioType = Encoding::PCM;

            // Read the header
            // TODO Could optimize to only read needed bytes if parsing is to slow
            WavHeader header = *reinterpret_cast<WavHeader*>(data + offset);
            offset += sizeof(header);

            // Find the number of samples
            soundData.sampleCount = (header.dataHeader.chunkSize / (header.formatHeader.bits_per_sample / 8));

            // TODO add caclulation for sample rate
            dataLength += soundData.sampleCount * sizeof(float);
            offset += header.dataHeader.chunkSize;

            table[id] = soundData;
        }
        return dataLength;
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

        char* fileBuffer = new char[fileSize];
        file.read(fileBuffer, fileSize);

        // Find start point of all data files
        int processedSize = GetDataStartAndLength(lookUpTable, fileBuffer, fileSize);

        // Create buffer to store PCM as floats in continues memory
        *data = new char[processedSize];

        ConvertToFloat(lookUpTable, *data);

        delete [] fileBuffer;
        return ErrorNum::NoErrors;
    }

}