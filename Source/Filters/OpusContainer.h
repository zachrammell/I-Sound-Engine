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
                                          offsetIntoOpusFrame(0),
                                          offsetIntoRawOpus(0)
    {

    }

    Frame<sampleType> GetNextSample() override
    {
        if(offsetIntoOpusFrame >= OpusFrameSize)
        {
            int tableIndex = 28;
            int opusPacketSize = *(opusData + offsetIntoRawOpus + tableIndex);
            ++tableIndex;
            int readSize = opusPacketSize;

            while(readSize == 255)
            {
                readSize = *(opusData + offsetIntoRawOpus + tableIndex);
                opusPacketSize += readSize;
                ++tableIndex;
            }
            offsetIntoOpusFrame += tableIndex;
            decoder.DecodeFloat(opusData + offsetIntoOpusFrame, opusPacketSize, decodedOpusFrame, OpusFrameSize);
            offsetIntoOpusFrame = 0;
        }

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

    float decodedOpusFrame[OpusFrameSize];
    int offsetIntoOpusFrame;

};

#endif //I_SOUND_ENGINE_OPUSCONTAINER_H
