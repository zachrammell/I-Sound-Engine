//
// Created by zack on 9/28/21.
//

#ifndef I_SOUND_ENGINE_SOUNDDATA_H
#define I_SOUND_ENGINE_SOUNDDATA_H

#include "inttypes.h"


enum Encoding
{
    PCM,
    Opus
};


struct SoundData
{
    char* data;
    uint32_t sampleCount;
    uint32_t sampleRate;
    Encoding audioType;
    uint8_t channels;
};

#endif //I_SOUND_ENGINE_SOUNDDATA_H
