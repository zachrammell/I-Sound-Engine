#include "PackageEncoder.h"
#include <fstream>
#include <memory>
#include <cstdint>

ErrorNum PackageEncoder::AddFile(WavFile& wav, uint32_t id, Encoding format)
{
    if(wav.GetError() == NoErrors)
    {
        filesToEncode.emplace_back(FileInfo{wav, id, format});
        int sampleCount = wav.GetDataSize() / (wav.GetFormat().bits_per_sample / 8);
        bufferSize += (sampleCount * sizeof (float)) + sizeof(uint32_t) + sizeof(WavHeader); // data size + id + header
    }
    return wav.GetError();
}

ErrorNum PackageEncoder::WritePackage(std::string path)
{
    //TODO get correct buffer size

    // Ensure bank can be made before doing work
    std::fstream bank(path.c_str(), std::ios_base::binary | std::ios_base::out);
    if(!bank.is_open())
        return ErrorNum::FailedToWriteFile;

    bank.write(reinterpret_cast<char*>(&bufferSize), sizeof(uint32_t));

    char* buffer = new char[bufferSize];
    uint32_t offset = 0;

    // Store all data in bank
    for(auto& file : filesToEncode)
    {
        // Write id
        *reinterpret_cast<uint32_t*>(buffer + offset) = file.id;
        offset += sizeof (uint32_t);

        // Write riff header
        RiffHeader riffHeader{{'R','I','F','F'},
                              0,
                              {'W','A','V','E'}};
        riffHeader.riff_size = sizeof(WavHeader) + file.wavFile.GetDataSize();
        *reinterpret_cast<RiffHeader*>(buffer + offset) = riffHeader;
        offset += sizeof(RiffHeader);

        // Write fmt header
        *reinterpret_cast<FormatHeader*>(buffer + offset) = file.wavFile.GetFormat();
        (*reinterpret_cast<FormatHeader*>(buffer + offset)).bits_per_sample = sizeof(float) * 8;
        offset += sizeof(FormatHeader);

        // TODO any proccesing like resmapleing would be done before this step

        // Write data header
        GenericHeaderChunk dataChunk{{'d', 'a','t','a'}, 0};
        dataChunk.chunkSize = (file.wavFile.GetDataSize() / (file.wavFile.GetFormat().bits_per_sample / 8)) * sizeof(float);
        *reinterpret_cast<GenericHeaderChunk*>(buffer + offset) = dataChunk;
        offset += sizeof (GenericHeaderChunk);

        // Write data
        file.wavFile.GetDataAsFloat(reinterpret_cast<float*>(buffer + offset));
        offset += dataChunk.chunkSize;
    }

    bank.write(buffer, offset);
    bank.close();
    delete [] buffer;


    return ErrorNum::NoErrors;
}