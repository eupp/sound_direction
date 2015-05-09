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
