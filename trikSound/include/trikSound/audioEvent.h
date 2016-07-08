/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
