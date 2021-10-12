//
// Created by zack on 10/10/21.
//

#ifndef I_SOUND_ENGINE_OPUSCONTAINER_H
#define I_SOUND_ENGINE_OPUSCONTAINER_H

#include "SoundContainer.h"
#include "OpusDecoderWrapper.h"
#include "OpusHeader.h"
#include "cstring"

constexpr int OpusFrameSize = 960;

template<typename sampleType>
class OpusContainer : public SoundContainer<sampleType>
{
public:

    OpusContainer(char* opusData, ChannelType channels) : type(channels),
                                          decoder(48000, channels == ChannelType::Mono ? 1 : 2),
                                          opusData(opusData),
                                          offsetIntoOpusFrame(std::numeric_limits<int>::max()),
                                          offsetIntoRawOpus(0)
    {

    }

    Frame<sampleType> GetNextSample() override
    {
        // Does a new frame need to be decoded
        if(offsetIntoOpusFrame >= OpusFrameSize * 2 )
        {
            // SanityCheck
            char OggSMagicNum[4];
            memcpy(OggSMagicNum, opusData + offsetIntoRawOpus, 4);
            if(OggSMagicNum[0] != 'O')
            {
                return Frame<sampleType>();
            }

            int tableIndex = 26; // Magic number to segmentation table
            int opusPacketSize = *reinterpret_cast<unsigned char*>((opusData + offsetIntoRawOpus + tableIndex));
            ++tableIndex;
            int readSize = opusPacketSize;

            // size == 255 means that there are more bytes need to create this segment
            while(readSize == 255)
            {
                readSize = *reinterpret_cast<unsigned char*>((opusData + offsetIntoRawOpus + tableIndex));
                opusPacketSize += readSize;
                ++tableIndex;
            }
            offsetIntoRawOpus += tableIndex;
            int returnValue = decoder.Decode(opusData + offsetIntoRawOpus, opusPacketSize, decodedOpusFrame, OpusFrameSize);
            if(returnValue < 0)
            {
                return Frame<sampleType>();
            }
            offsetIntoRawOpus += opusPacketSize;
            offsetIntoOpusFrame = 0;
        }

        // Read samples into output
        Frame<sampleType> output;
        if(type == ChannelType::Mono)
        {
            output.leftChannel = decodedOpusFrame[offsetIntoOpusFrame];
            output.rightChannel = decodedOpusFrame[offsetIntoOpusFrame];
            ++offsetIntoOpusFrame;
        }
        else
        {
            output.leftChannel = decodedOpusFrame[offsetIntoOpusFrame];
            ++offsetIntoOpusFrame;
            output.rightChannel = decodedOpusFrame[offsetIntoOpusFrame];
            ++offsetIntoOpusFrame;
        }
        return output;
    }

private:
    ChannelType type;
    OpusDecoderWrapper decoder;

    char* opusData;
    uint64_t offsetIntoRawOpus;

    short decodedOpusFrame[OpusFrameSize * 2];
    int offsetIntoOpusFrame;

};

#endif //I_SOUND_ENGINE_OPUSCONTAINER_H
