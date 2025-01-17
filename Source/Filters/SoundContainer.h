/******************************************************************************/
/*!
\file   SoundContainer.h
\author Zack Krolikowksi
\date   8/22/2021

    Abstract class for all sound encodings to inhearent from
*/
/******************************************************************************/
#ifndef I_SOUND_ENGINE_SOUNDCONTAINER_H
#define I_SOUND_ENGINE_SOUNDCONTAINER_H

#include <cmath>

// TODO find a proper location for sample
/*!
 * Left and right samples for given index
 */
template <typename sampleType>
struct Frame
{
    sampleType leftChannel;  //! Sample for left channel
    sampleType rightChannel; //! Sample for right Channel
};

// TODO find a proper location for channel type
/*!
 * The types of channels for a given sound
 */
enum ChannelType
{
  Mono = 1,
  Stereo = 2
};

template<typename sampleType>
class SoundContainer
{
public:
    /*!
     * Ctor, init playbackModifier speed to 1 so sound plays at oringal speed
     */
    SoundContainer() :
                        playbackModifier(static_cast<sampleType>(1)) {}

    /*!
     * Gets the next sample from current position based on play back speed
     * @return The next sample
     */
    virtual Frame<sampleType> GetNextSample()                 = 0;

    /*!
     * Gets a sample from current position to offset based on play back speed
     * @param offset How far to move from current position
     * @return The sample at the position
     */
    //virtual Frame<sampleType> GetSampleFromOffset(int offset) = 0;

    /*!
     * Sets the playback speed
     *   1.0 = Original speed
     *  >1.0 = Faster play back speed
     *  <1.0 = Slower play back speed
     * @param modifier The new play back speed
     */
    void setPlayBackSpeed(sampleType modifier)
    {
        playbackModifier = modifier;
    }

    /*!
     * Adjust the original pitch by the given cents
     * @param cents Adjustment to original pitch
     */
    void setPitch(int cents)
    {
        //TODO find method that doesnt use pow
        playbackModifier = static_cast<sampleType>(std::pow(static_cast<sampleType>(2),
                                                            static_cast<sampleType>(cents) / 1200));
    }

    /*!
     * Set the position to seek to
     * @param position position to seek to
     */
    //virtual void Seek(int position)                    = 0;
    //virtural const storageType& GetAllSamples()      = 0;

protected:
    /*!
     * The playback modifier to control speed of sound playback
     */
    sampleType playbackModifier;
};

#endif //I_SOUND_ENGINE_SOUNDCONTAINER_H
