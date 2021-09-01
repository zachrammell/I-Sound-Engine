//
// Created by zacke on 8/31/2021.
//

#ifndef I_SOUND_ENGINE_WAVFILE_H
#define I_SOUND_ENGINE_WAVFILE_H

#include <string>
#include <fstream>
#include "WavHeader.h"
#include "../ErrorList.h"

class WavFile
{
public:
    /*!
     * Ctor for wav object wrapper.
     * @param path Path to a wav file to use
     */
    WavFile(std::string path);

    /*!
     * Checks returns wether wav file is valid or not
     * @return True - valid wav file, False otherwise
     */
    operator bool() const;

    /*!
     * Retunrs the error state
     * @return Error
     */
    ErrorNum GetError() const;

    /*!
     * Get the format chunk of the wav file
     * @return Format chunk
     */
    const FormatHeader& getFormat() const;

    /*!
     * Fills given buffer with data, make sure buffer is big enough to store all samples
     * @tparam sampleType Check format chuck to get native type
     * @param buffer buffer to fill
     * @return true - read was successful
     */
    template<typename sampleType>
    bool GetDataInNativeType(std::unique_ptr<sampleType[]> buffer);

    /*!
     * Fills buffer with samples in range [-1, 1]
     * @tparam castedType Double or float
     * @param buffer buffer to be filled
     * @return true - read was successful
     */
    template<typename castedType>
    bool GetDataInDecimal(std::unique_ptr<castedType[]> buffer);

private:
    ErrorNum errorState;
    unsigned dataPosition;
    unsigned dataSize;
    std::fstream wavFile;
    FormatHeader fmtHeader;
};

#endif //I_SOUND_ENGINE_WAVFILE_H
