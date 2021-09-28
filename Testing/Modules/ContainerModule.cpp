//
// Created by zack on 9/26/21.
//
#include "ContainerModule.h"
#include <string>
#include "WavFile.h"

void readWaveFile(std::string name)
{
    //Opening test
    const char* path = name.c_str();
    WavFile wavFile(name);
    //if(!wavFile)
    ASSERT_FALSE(!wavFile) << "Failed to open file named ";


    // Conversion Test
    int samples = wavFile.GetDataSize() / (wavFile.GetFormat().bits_per_sample / 8);

    // Get Converted data
    std::unique_ptr<float*> floatSamples = std::make_unique<float*>(new float[samples]);
    uint32_t dataSize = samples * sizeof(float);

    char* someData = new char[wavFile.GetDataSize()];
    std::unique_ptr<char*> nativeSamples = std::make_unique<char*>(someData);

    wavFile.GetDataAsFloat(*floatSamples);

    wavFile.GetDataInNativeType(*nativeSamples);

    switch (wavFile.GetFormat().bits_per_sample)
    {
        case 8:
            for(int i = 0; i < samples; ++i)
            {
                float f = (*((*floatSamples.get()) + i));
                short fValue = (f * (127)) ;
                fValue += (128);

                unsigned char sampleValue = *((*nativeSamples.get()) + i);
                ASSERT_TRUE((fValue >= sampleValue -1 && fValue <= sampleValue + 1)) << "Float conversion failed";
            }
            break;
        case 16:
            for(int i = 0; i < samples; ++i)
            {
                float f = (*floatSamples)[i];
                short fValue = f * ((1<<15) - 1);

                short sampleValue = reinterpret_cast<short*>((*nativeSamples))[i];
                ASSERT_TRUE((fValue >= sampleValue -1 && fValue <= sampleValue + 1)) << "Float conversion failed";
            }
            break;

        default:
            ASSERT_FALSE(true) << "Unsupported file format";
            return;
    }

    return;
}