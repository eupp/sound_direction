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

#include "triksound_global.h"
#include "vadFilter.h"
#include "stereoVadFilter.h"

namespace trikSound
{

template <typename Iter>
class VadFilterWrapper
{
public:

    enum class VadType
    {
        MONO
      , STEREO
    };

    typedef std::shared_ptr<VadFilter<Iter>>          VadFilterPtr;
    typedef std::shared_ptr<StereoVadFilter<Iter>>    StereoVadFilterPtr;

    VadFilterWrapper(const VadFilterPtr& vad);
    VadFilterWrapper(const StereoVadFilterPtr& vad);

    double getEnergyCoefficient() const;

    bool isActive() const;

    VadType getType() const;

    VadFilterPtr getMonoVad() const;
    StereoVadFilterPtr getStereoVad() const;

private:

    VadFilterPtr        mMonoVad;
    StereoVadFilterPtr  mStereoVad;
};

template <typename Iter>
VadFilterWrapper<Iter>::VadFilterWrapper(const VadFilterWrapper<Iter>::VadFilterPtr& vad):
    mMonoVad(vad)
  , mStereoVad()
{}

template <typename Iter>
VadFilterWrapper<Iter>::VadFilterWrapper(const VadFilterWrapper<Iter>::StereoVadFilterPtr& vad):
    mMonoVad()
  , mStereoVad(vad)
{}


template <typename Iter>
double VadFilterWrapper<Iter>::getEnergyCoefficient() const
{
    if (mMonoVad) {
        return mMonoVad->getEnergyCoefficient();
    }
    else if (mStereoVad) {
        return mStereoVad->getEnergyCoefficient();
    }
}

template <typename Iter>
bool VadFilterWrapper<Iter>::isActive() const
{
    if (mMonoVad) {
        return mMonoVad->isActive();
    }
    else if (mStereoVad) {
        return mStereoVad->isActive();
    }
}

template <typename Iter>
typename VadFilterWrapper<Iter>::VadType VadFilterWrapper<Iter>::getType() const
{
    if (mMonoVad) {
        return VadType::MONO;
    }
    else if (mStereoVad) {
        return VadType::STEREO;
    }
}

template <typename Iter>
typename VadFilterWrapper<Iter>::VadFilterPtr VadFilterWrapper<Iter>::getMonoVad() const
{
    return mMonoVad;
}

template <typename Iter>
typename VadFilterWrapper<Iter>::StereoVadFilterPtr VadFilterWrapper<Iter>::getStereoVad() const
{
    return mStereoVad;
}

}
