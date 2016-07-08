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

#include <memory>
#include <algorithm>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "vadFilter.h"
#include "types.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT StereoVadFilter: public StereoAudioFilter<Iter>
{
public:

    using typename StereoAudioFilter<Iter>::FilterPtr;
    using typename StereoAudioFilter<Iter>::range_type;

    explicit StereoVadFilter(threshold_type threshold);

    threshold_type getEnergyCoefficient() const;

    bool isActive() const;

    bool framePromotion() const
    {
        return isActive();
    }

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:

    typedef std::unique_ptr<VadFilter<Iter>> VadFilterPtr;

    VadFilterPtr mLeftVad;
    VadFilterPtr mRightVad;
};

template <typename Iter>
StereoVadFilter<Iter>::StereoVadFilter(threshold_type threshold):
    mLeftVad(new VadFilter<Iter>(threshold))
  , mRightVad(new VadFilter<Iter>(threshold))
{}

template <typename Iter>
threshold_type StereoVadFilter<Iter>::getEnergyCoefficient() const
{
    return std::max(mLeftVad->getEnergyCoefficient(), mRightVad->getEnergyCoefficient());
}

template <typename Iter>
bool StereoVadFilter<Iter>::isActive() const
{
    return mLeftVad->isActive() || mRightVad->isActive();
}

template <typename Iter>
void StereoVadFilter<Iter>::handleWindowImpl(range_type channel1, range_type channel2)
{
    mLeftVad->handleWindow(channel1.first, channel1.second);
    mRightVad->handleWindow(channel2.first, channel2.second);
}

}
