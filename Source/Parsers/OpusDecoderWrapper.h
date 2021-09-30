//
// Created by zack on 9/29/21.
//

#ifndef I_SOUND_ENGINE_OPUSDECODERWRAPPER_H
#define I_SOUND_ENGINE_OPUSDECODERWRAPPER_H

#include "opus.h"

class OpusDecoderWrapper
{
public:
    //TODO How to get channel count???
    explicit OpusDecoderWrapper(int sampleRate, int channels);

    int Decode(char* input, char* output, int frameSize);

private:
    OpusDecoder* decoder = nullptr;
    int error = 0;
};


#endif //I_SOUND_ENGINE_OPUSDECODERWRAPPER_H
