//
// Created by zacke on 8/31/2021.
//

#include "WavFile.h"
#include <fstream>

WavFile::WavFile(const std::string& path) : errorState(ErrorNum::NoErrors),
                                     wavFile(path, std::ios_base::binary | std::ios_base::in),
                                     fmtHeader()
{
    // Open file
    if(!wavFile)
    {
        errorState = ErrorNum::FailedToFindFile;
        return;
    }

    // Read the wav header chunk
    char header[sizeof(RiffHeader)];
    wavFile.read(header, sizeof(RiffHeader));
    RiffHeader riffHeader = *reinterpret_cast<RiffHeader*>(header);

    // Check if is a riff file
    if(riffHeader.riff_label[0] != 'R' ||
       riffHeader.riff_label[1] != 'I' ||
       riffHeader.riff_label[2] != 'F' ||
       riffHeader.riff_label[3] != 'F')
    {
        errorState = ErrorNum::NotAPCMFile;
        return;
    }

    // Check if is a wav file
    if(riffHeader.file_tag[0] != 'W' ||
       riffHeader.file_tag[1] != 'A' ||
       riffHeader.file_tag[2] != 'V' ||
       riffHeader.file_tag[3] != 'E')
    {
        errorState = ErrorNum::NotAPCMFile;
        return;
    }

    // Find Format Chunk
    GenericHeaderChunk chunk {};
    wavFile.read(reinterpret_cast<char*>(&chunk), sizeof(GenericHeaderChunk));
    while(wavFile && (chunk.label[0] != 'f' ||
                      chunk.label[1] != 'm' ||
                      chunk.label[2] != 't' ||
                      chunk.label[3] != ' ' ))
    {
        wavFile.seekg(chunk.chunkSize, std::ios_base::cur);
        wavFile.read(reinterpret_cast<char*>(&chunk), sizeof(GenericHeaderChunk));
    }
    if(!wavFile)
    {
        errorState = ErrorNum::NotAPCMFile;
        return;
    }

    //Read fmt chunk and verify
    wavFile.seekg(-sizeof(GenericHeaderChunk), std::ios_base::cur);
    wavFile.read(reinterpret_cast<char*>(&fmtHeader), sizeof(fmtHeader));

    if(fmtHeader.audio_format != 1)
    {
        errorState = ErrorNum::UnsportedFile;
        return;
    }

    if(fmtHeader.bytes_per_sample * 8 != fmtHeader.bits_per_sample)
    {
        errorState = ErrorNum::PossibleCorution;
        return;
    }

    // Find data Chunk
    wavFile.read(reinterpret_cast<char*>(&chunk), sizeof(GenericHeaderChunk));
    while(wavFile && (chunk.label[0] != 'd' ||
                      chunk.label[1] != 'a' ||
                      chunk.label[2] != 't' ||
                      chunk.label[3] != 'a' ))
    {
        wavFile.seekg(chunk.chunkSize, std::ios_base::cur);
        wavFile.read(reinterpret_cast<char*>(&chunk), sizeof(GenericHeaderChunk));
    }

    dataPosition = wavFile.tellg();
    dataSize = chunk.chunkSize;
}

WavFile::operator bool() const
{
    if(errorState == ErrorNum::NoErrors)
        return true;
    return false;
}

ErrorNum WavFile::GetError() const
{
    return errorState;
}

const FormatHeader& WavFile::getFormat() const
{

    return fmtHeader;
}

template<typename sampleType>
bool WavFile::GetDataInNativeType(std::unique_ptr<sampleType[]> buffer)
{
    wavFile.seekg(dataPosition, std::ios_base::beg);
    wavFile.read(reinterpret_cast<char*>(buffer.get()), dataSize * sizeof(sampleType));
    return true;
}

template<typename castedType>
bool WavFile::GetDataInDecimal(std::unique_ptr<castedType[]> buffer)
{
    /* Idea
     * float - smallest decmal is atleast 4 bytes
     * short - 2 bytes
     * Read data samples into second half of the buffer then
     * start to convert samples at the start
     */
                                // buffer size                       native data size
    unsigned readPointer  = ((sizeof(castedType) * dataSize) - (dataSize * fmtHeader.bytes_per_sample));  // in number of bytes
    unsigned writePos     = 0; // first index

    wavFile.seekg(dataPosition, std::ios_base::beg);
    wavFile.read(reinterpret_cast<char*>(buffer.get()) + readPointer, dataSize * fmtHeader.bytes_per_sample);

    switch(fmtHeader.bytes_per_sample)
    {
        // 8 bit conversion
        case 1:
            for(unsigned i = dataSize; --i >= 0;)
            {
                buffer[writePos] = ((*reinterpret_cast<char*>(buffer.get() + readPointer))
                                    - std::numeric_limits<char>::max()) /std::numeric_limits<unsigned char>::max();
                ++writePos;
                ++readPointer;
            }
            break;
        // 16 bit conversion
        case 2:
            for(unsigned i = dataSize; --i >= 0;)
            {
                buffer[writePos] = (*reinterpret_cast<short*>(buffer.get()) + (readPointer / sizeof(short)))
                                    / static_cast<castedType>(std::numeric_limits<short>::max());

                ++writePos;
                readPointer += sizeof(short);
            }
            break;
    }
}