#ifndef I_SOUND_ENGINE_MEMORYMAPPEDFILE_H
#define I_SOUND_ENGINE_MEMORYMAPPEDFILE_H

#include <string>

namespace IO
{

class MemoryMappedFile
{
public:
  MemoryMappedFile(std::string const& filepath);
  MemoryMappedFile(char const* filepath);
  ~MemoryMappedFile();

  size_t Size()
  {
    return size;
  }
  char const* Data()
  {
    return data;
  }
private:
  size_t size;
  char const* data;
};

}

#endif // I_SOUND_ENGINE_MEMORYMAPPEDFILE_H
