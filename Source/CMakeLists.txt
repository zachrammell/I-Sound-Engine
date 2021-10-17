set(ISE_SOURCES
  "AudioFormats/OpusDecoderWrapper.h"
  "AudioFormats/OpusDecoderWrapper.cpp"
  "AudioFormats/OpusEncoderWrapper.h"
  "AudioFormats/OpusEncoderWrapper.cpp"
  "AudioFormats/OpusFile.h"
  "AudioFormats/OpusFile.cpp"
  "AudioFormats/OpusHeader.h"
  "AudioFormats/WavFile.h"
  "AudioFormats/WavFile.cpp"
  "AudioFormats/WavHeader.h"
  "AudioPackage/PackageDecoder.h"
  "AudioPackage/PackageDecoder.cpp"
  "AudioPackage/PackageEncoder.h"
  "AudioPackage/PackageEncoder.cpp"
  "Filters/OpusContainer.h"
  "Filters/SoundContainer.h"
  "Filters/WavContainer.h"
  "ErrorList.h"
  "SoundData.h"
)

add_library(ISoundEngine ${ISE_SOURCES})
target_include_directories(ISoundEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ISoundEngine opus)

