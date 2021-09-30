//
// Created by zack on 9/29/21.
//

#include "OpusFile.h"

#include <iostream>

OpusFile::OpusFile(const std::string &path) : errorState(ErrorNum::NoErrors),
                                              opusFile(path, std::ios_base::binary | std::ios_base::in)
{
    if(!opusFile)
    {
        errorState = ErrorNum::FailedToFindFile;
        return;
    }

    opusFile.read(reinterpret_cast<char*>(&opusHeader), sizeof(opusHeader) - 1);
    // TODO Read memory mapping

    opusFile.read(commentHeader.signature, sizeof(commentHeader.signature));
    opusFile.read(reinterpret_cast<char*>(&commentHeader.vendorStringLength), sizeof(unsigned ));
    if(commentHeader.vendorStringLength > 0)
    {
        commentHeader.vendorString = new char[commentHeader.vendorStringLength];
        opusFile.read(commentHeader.vendorString, commentHeader.vendorStringLength);
    }

    opusFile.read(reinterpret_cast<char*>(&commentHeader.numOfUsercomments), sizeof(commentHeader.numOfUsercomments));
    if(commentHeader.numOfUsercomments > 0)
    {
        commentHeader.userCommentsLength = new unsigned[commentHeader.numOfUsercomments];
        commentHeader.userComments = new char*[commentHeader.numOfUsercomments];
        for(int i = 0; i < commentHeader.numOfUsercomments; ++i)
        {
            opusFile.read(reinterpret_cast<char*>(commentHeader.userCommentsLength + i), sizeof(unsigned));
            commentHeader.userComments[i] = new char[commentHeader.userCommentsLength[i]];
            opusFile.read(commentHeader.userComments[i], commentHeader.userCommentsLength[i]);
        }
    }

    //TODO any work depending on user comments before clean up

    for(int i = 0; i < commentHeader.numOfUsercomments; ++i)
    {
        std::cout << commentHeader.userComments[i] << std::endl;
    }
}

OpusFile::~OpusFile()
{
    if(commentHeader.vendorStringLength > 0)
        delete [] commentHeader.vendorString;


    for(int i = 0; i < commentHeader.numOfUsercomments; ++i)
    {
        delete [] commentHeader.userComments[i];
    }

    if(commentHeader.numOfUsercomments > 0)
    {
        delete [] commentHeader.userComments;
        delete [] commentHeader.userCommentsLength;
    }
}