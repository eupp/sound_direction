#pragma once

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioEvent
{
public:

    double angle() const;
    void setAngle(double newAngle);

private:
    double mAngle;
};

inline double AudioEvent::angle() const
{
    return mAngle;
}

inline void AudioEvent::setAngle(double newAngle)
{
    mAngle = newAngle;
}


}
