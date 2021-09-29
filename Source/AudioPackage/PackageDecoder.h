//
// Created by zack on 9/23/21.
//

#ifndef I_SOUND_ENGINE_PACKAGEDECODER_H
#define I_SOUND_ENGINE_PACKAGEDECODER_H

#include "../ErrorList.h"
#include "../SoundData.h"
#include <unordered_map>

namespace PackageDecoder
{
    ErrorNum DecodePackage(std::unordered_map<uint64_t, SoundData>& lookUpTable, char** data, std::string path);
};

#endif //I_SOUND_ENGINE_PACKAGEDECODER_H
