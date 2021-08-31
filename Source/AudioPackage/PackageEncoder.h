
#ifndef I_SOUND_ENGINE_PACKAGEENCODER_H
#define I_SOUND_ENGINE_PACKAGEENCODER_H

#include <vector>
#include <string>

// TODO make system to handle all errors
enum ErrorNum
{
    NoErrors,
    FailedToFindFile,
    FailedToWriteFile
};

enum Format
{
    PCM,
    Opus
};

class PackageEncoder
{
public:

    //PackageEncoder();

    /*!
     * Adds a new file to the bank
     * @param path The filepath the audio file
     * @param id Id that events will refer to this audio file as
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
    std::vector<std::tuple<unsigned, Format, std::string>> filePaths;
};


#endif //I_SOUND_ENGINE_PACKAGEENCODER_H
