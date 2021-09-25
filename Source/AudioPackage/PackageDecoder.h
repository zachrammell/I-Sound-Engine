//
// Created by zack on 9/23/21.
//

#ifndef I_SOUND_ENGINE_PACKAGEDECODER_H
#define I_SOUND_ENGINE_PACKAGEDECODER_H
#include "../ErrorList.h"

namespace PackageDecoder
{
    ErrorNum DecodePackage(char* data, std::string path);
};


#endif //I_SOUND_ENGINE_PACKAGEDECODER_H
