#include "../WavFile.h"
#include <cassert>
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "No File path given" << std::endl;
        return 0;
    }


    //Opening test
    WavFile wavFile(argv[1]);
    if(!wavFile)
    {
        std::cout << "Error: Failed to open and read file \"" << argv[1] << "\"" << std::endl;
        assert(! "Failed to open and read a file");
    }

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
                if(!(fValue >= sampleValue -1 && fValue <= sampleValue + 1))
                {
                    assert(! " Float conversion failed");
                    return  0;
                }
            }
            break;
        case 16:
            for(int i = 0; i < samples; ++i)
            {
                float f = (*floatSamples)[i];
                short fValue = f * ((1<<15) - 1);

                short sampleValue = reinterpret_cast<short*>((*nativeSamples))[i];
                if(!(fValue >= sampleValue -1 && fValue <= sampleValue + 1))
                {
                    assert(! " Float conversion failed");
                    return  0;
                }
            }
            break;

        default:
            assert(! "Unsupported file format");
            return 0;
    }

    return 0;
}