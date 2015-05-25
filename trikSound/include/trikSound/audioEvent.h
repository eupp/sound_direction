#pragma once

#include <ctime>

#include "triksound_global.h"
#include "vadFilter.h"
#include "types.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioEvent
{
public:

    AudioEvent();

    double angle() const;
    void setAngle(double newAngle);
    bool angleSetFlag() const;


    threshold_type vadCoef() const;
    void setVadCoef(threshold_type vadCoef);
    bool vadCoefSetFlag() const;

    bool vadIsActive() const;
    void setVadIsActive(bool vadIsActive);
    bool vadIsActiveSetFlag() const;

    clock_t timestamp() const;

private:

    bool mAngleSetFlag;
    double mAngle;

    bool mVadCoefSetFlag;
    double mVadCoef;

    bool mVadIsActiveSetFlag;
    bool mVadIsActive;

    clock_t mTimestamp;
};

inline AudioEvent::AudioEvent():
    mAngle(0)
  , mAngleSetFlag(false)

  , mVadCoef(0)
  , mVadCoefSetFlag(false)

  , mTimestamp(clock())
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

inline threshold_type AudioEvent::vadCoef() const
{
    return mVadCoef;
}

inline void AudioEvent::setVadCoef(threshold_type vadCoef)
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

inline clock_t AudioEvent::timestamp() const
{
    return mTimestamp;
}



}
