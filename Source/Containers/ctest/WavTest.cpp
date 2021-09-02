#include "../WavFile.h"
#include <assert.h>

int main(int argc, char* argv[])
{
    WavFile wavFile(argv[1]);
    assert(wavFile == true);
    return 0;
}