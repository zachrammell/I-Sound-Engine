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
    uint32_t sampleCount;
    char* data;
    Encoding audioType;
};

#endif //I_SOUND_ENGINE_SOUNDDATA_H
