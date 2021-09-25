//
// Created by zack on 9/23/21.
//

#include <fstream>
#include "../Containers/WavHeader.h"
#include <unordered_map>

namespace PackageDecoder
{
    int GetDataStartAndLength(std::unordered_map<uint32_t, char*>& table, char* data, int size)
    {
        int DataLength = 0;
        for(int offset = 0; offset < size;)
        {
            uint32_t id = *reinterpret_cast<uint32_t*>(data + offset);
            offset += sizeof(uint32_t);

            table[id] = data + offset;

            // TODO Could optimize to only read needed bytes if parsing is to slow
            WavHeader header = *reinterpret_cast<WavHeader*>(data + offset);
            offset += sizeof(header);

            // TODO add caclulation for sample rate
            dataLength += (header.dataHeader.chunkSize / (header.formatHeader.bits_per_sample / 8)) * sizeof(float_t);
            offset += header.dataHeader.chunkSize;
        }
    }

    void Convert16Bit(int size, short* in, float* out)
    {
        for(int i = 0; i < size; ++i)
        {
            out[i] = in[i] / static_cast<float>(std::numeric_limits<short>::max());
        }
    }

    void Convert8Bit(int size, char* in, float* out)
    {
        for(int i = 0; i < size; ++i)
        {
            out[i] = (in[i] - std::numeric_limits<char>::max()) / static_cast<float>(std::numeric_limits<char>::max())
        }
    }

    void ConvertToFloat(std::unordered_map<uint32_t, char*> table, char* fileBuffer, char* outputBuffer)
    {
        for(char*& file : table)
        {
            WavHeader header = *reinterpret_cast<WavHeader>(file);

        }
    }

    ErrorNum DecodePackage(std::unordered_map<uint32_t, char*>& lookUpTable, char** data, std::string path)
    {
        std::fstream file(path, std::ios_base::binary | std::ios_base::in);
        if(!file.is_open())
            return  ErrorNum::FailedToFindFile;

        uint32_t fileSize;
        file.read(reinterpret_cast<char*>(&fileSize), sizeof(uint32_t));

        char* fileBuffer = new char[fileSize];
        file.read(fileBuffer, fileSize);

        int processedSize = GetDataStartAndLength(lookUpTable, fileBuffer, fileSize);

        *data = new char[processedSize];

    }

}