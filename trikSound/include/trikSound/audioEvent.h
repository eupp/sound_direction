#pragma once

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioEvent
{
public:

    AudioEvent();

    double angle() const;
    void setAngle(double newAngle);
    bool angleSetFlag() const;


    double vadCoef() const;
    void setVadCoef(double vadCoef);
    bool vadCoefSetFlag() const;

    bool vadIsActive() const;
    void setVadIsActive(bool vadIsActive);
    bool vadIsActiveSetFlag() const;


private:

    bool mAngleSetFlag;
    double mAngle;

    bool mVadCoefSetFlag;
    double mVadCoef;

    bool mVadIsActiveSetFlag;
    bool mVadIsActive;
};

inline AudioEvent::AudioEvent():
    mAngle(0)
  , mAngleSetFlag(false)

  , mVadCoef(0)
  , mVadCoefSetFlag(false)
{}

inline double AudioEvent::angle() const
{
    return mAngle;
}

inline void AudioEvent::setAngle(double newAngle)
{
    mAngleSetFlag = true;
    mAngle = newAngle;
}

inline bool AudioEvent::angleSetFlag() const
{
    return mAngleSetFlag;
}

inline double AudioEvent::vadCoef() const
{
    return mVadCoef;
}

inline void AudioEvent::setVadCoef(double vadCoef)
{
    mVadCoefSetFlag = true;
    mVadCoef = vadCoef;
}

inline bool AudioEvent::vadCoefSetFlag() const
{
    return mVadCoefSetFlag;
}

inline bool AudioEvent::vadIsActive() const
{
    return mVadIsActive;
}

inline void AudioEvent::setVadIsActive(bool vadIsActive)
{
    mVadIsActiveSetFlag = true;
    mVadIsActive = vadIsActive;
}

inline bool AudioEvent::vadIsActiveSetFlag() const
{
    return mVadIsActiveSetFlag;
}


}
