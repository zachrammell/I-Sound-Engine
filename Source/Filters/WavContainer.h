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
#include <memory>


template<typename sampleType>
class WavContainer : public SoundContainer<sampleType>
{
public:
    WavContainer(const std::unique_ptr<sampleType*>, ChannelType type) : index(0), channelType(type)
    {

    }

    // Fully contaioned sound object
    //WavContainer()


    Sample<sampleType> GetNextSample()
    {
        return 0.0f;
    }

    Sample<sampleType> GetSampleFromOffset(int offset)
    {
        return 0.0f;
    }

    void Seek(int position)
    {

    }

private:
    int index;
    ChannelType channelType;
};

#endif //I_SOUND_ENGINE_WAVCONTAINER_H
