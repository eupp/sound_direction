#pragma once

#include <memory>
#include <algorithm>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "vadFilter.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT StereoVadFilter: public StereoAudioFilter<Iter>
{
public:

    using typename StereoAudioFilter<Iter>::FilterPtr;
    using typename StereoAudioFilter<Iter>::range_type;


    explicit StereoVadFilter(double threshold);

    double getEnergyCoefficient() const;

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
StereoVadFilter<Iter>::StereoVadFilter(double threshold):
    mLeftVad(new VadFilter<Iter>(threshold))
  , mRightVad(new VadFilter<Iter>(threshold))
{}

template <typename Iter>
double StereoVadFilter<Iter>::getEnergyCoefficient() const
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
