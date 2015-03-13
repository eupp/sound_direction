#pragma once

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioEvent
{
public:

    double angle() const;
    void setAngle(double angle);

private:
    double mAngle;
};

double AudioEvent::angle() const
{
    return mAngle;
}

void AudioEvent::setAngle(double angle)
{
    mAngle = angle;
}


}
