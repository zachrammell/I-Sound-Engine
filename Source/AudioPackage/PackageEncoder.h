
#ifndef I_SOUND_ENGINE_PACKAGEENCODER_H
#define I_SOUND_ENGINE_PACKAGEENCODER_H

#include <vector>
#include <string>
#include "../Containers/WavHeader.h" //fmt header
#include "../ErrorList.h"

// TODO make system to handle all errors


enum Encoding
{
    PCM,
    Opus
};

class PackageEncoder
{
public:

    //PackageEncoder();

    /*!
     * Adds a new pcm(wav) file to the bank
     * @param path The filepath the audio file
     * @param id Id that events will refer to this audio file as
     * @param format Wether file should be encoded to be in opus or pcm
     * @return Error
     */
     ErrorNum AddFile(std::string path, unsigned id, Format format);

     /*!
      * Generates the entire bank then writes it
      * @param path The output path + file name for the bank
      * @return Error
      */
     ErrorNum WritePackage(std::string path);

private:
    unsigned bufferSize;

    struct FileInfo
    {
        std::string path;
        unsigned id;
        Encoding encoding;
        FormatHeader format;
    };

    std::vector<FileInfo> filePaths;
};


#endif //I_SOUND_ENGINE_PACKAGEENCODER_H
