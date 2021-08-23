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

template<typename sampleType>
class SoundContainer
{
public:
    /*!
     * Gets the next sample from current position based on play back speed
     * @return The next sample
     */
    virtual sampleType GetNextSample()                 = 0;

    /*!
     * Gets a sample from current position to offset based on play back speed
     * @param offset How far to move from current position
     * @return The sample at the position
     */
    virtual sampleType GetSampleFromOffset(int offset) = 0;

    /*!
     * Sets the playback speed
     *   1.0 = Original speed
     *  >1.0 = Faster play back speed
     *  <1.0 = Slower play back speed
     * @param modifier The new play back speed
     */
    virtual void setPlayBackSpeed(sampleType modifier) = 0;

    /*!
     * Adjust the original pitch by the given cents
     * @param cents Adjustment to original pitch
     */
    virtual void setPitch(int cents)                   = 0;

    /*!
     * Set the position to seek to
     * @param position position to seek to
     */
    virtual void Seek(int position)                    = 0;
    //virtural const storageType& GetAllSamples()      = 0;
};

#endif //I_SOUND_ENGINE_SOUNDCONTAINER_H
