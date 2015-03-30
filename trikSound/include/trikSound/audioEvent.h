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
    void setAngleSetFlag(bool value);

private:
    bool mAngleSetFlag;
    double mAngle;
};

inline AudioEvent::AudioEvent():
    mAngle(0)
  , mAngleSetFlag(false)
{

}

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



}
