//
// Created by zack on 9/23/21.
//

#include "../PackageEncoder.h"
#include <cassert>

int main(int argc, char* argv[])
{
    std::vector<WavFile> files;
    for(int i = 2; i < argc; ++i)
    {
        //WavFile wav(argv[i]);
        files.emplace_back(WavFile{argv[i]});
        if(files[i - 2].GetError() != ErrorNum::NoErrors)
            assert(! " Unable to open file");
    }

    PackageEncoder encoder;

    for(int i = 0; i < files.size(); ++i)
    {
        encoder.AddFile(files[i], i, Encoding::PCM);
    }

    encoder.WritePackage(argv[1]);

}