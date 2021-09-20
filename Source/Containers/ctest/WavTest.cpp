#include "../WavFile.h"
#include <assert.h>
#include <iostream>

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
    int samples = wavFile.GetDataSize() / (wavFile.getFormat().bits_per_sample / 8);

    // Get Converted data
    std::unique_ptr<float*> floatSamples = std::make_unique<float*>(new float[samples]);
    wavFile.GetDataAsFloat(*floatSamples);

    uint32_t dataSize = wavFile.GetDataSize();
    char* someData = new char[dataSize]();
    std::unique_ptr<char*> data = std::make_unique<char*>(someData);
    wavFile.GetDataInNativeType(*data.get());

    switch (wavFile.getFormat().bits_per_sample)
    {
        case 8:
            for(int i = 0; i < samples; ++i)
            {
                float f = (*((*floatSamples.get()) + i));
                short fValue = (f * (127)) ;
                      fValue += (128);

                unsigned char sampleValue = *((*data.get()) + i);
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