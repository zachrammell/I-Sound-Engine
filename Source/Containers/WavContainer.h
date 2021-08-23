/******************************************************************************/
/*!
\file   WavContainer.h
\author Zack Krolikowksi
\date   8/22/2021

    Container for wav files
*/
/******************************************************************************/

#ifndef I_SOUND_ENGINE_WAVCONTAINER_H
#define I_SOUND_ENGINE_WAVCONTAINER_H

#include "SoundContainer.h"

template<typename sampleType>
class WavContainer : public SoundContainer<sampleType>
{
public:
    WavContainer()
    {

    }

    sampleType GetNextSample()
    {
        return 0.0f;
    }

    sampleType GetSampleFromOffset(int offset)
    {
        return 0.0f;
    }

    void setPlayBackSpeed(sampleType modifier)
    {

    }

    void setPitch(int cents)
    {

    }

    void Seek(int position)
    {

    }
};

#endif //I_SOUND_ENGINE_WAVCONTAINER_H
