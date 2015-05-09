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

private:

    bool mAngleSetFlag;
    double mAngle;

    bool mVadCoefSetFlag;
    double mVadCoef;
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

}
