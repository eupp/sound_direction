#pragma once

#include <memory>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "utils.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT SplitFilter : public StereoAudioFilter<Iter>
{
public:

    SplitFilter(const AudioFilter::ptrFilter& channelFilter = AudioFilter::ptrFilter(),
                const StereoAudioFilter::ptrFilter& prevFilter = StereoAudioFilter::ptrFilter());

protected:

    void handleWindowImpl(StereoAudioFilter::range_type channel1,
                          StereoAudioFilter::range_type channel2);

private:
    AudioFilter::ptrFilter mChannelFilter;
};

template <typename Iter>
SplitFilter::SplitFilter(const StereoAudioFilter::ptrFilter& channelFilter,
                         const StereoAudioFilter::ptrFilter& prevFilter):
    StereoAudioFilter(prevFilter)
  , mChannelFilter(channelFilter)
{}

template <typename Iter>
void SplitFilter::handleWindowImpl(StereoAudioFilter::range_type channel1,
                                   StereoAudioFilter::range_type channel2)
{
    mChannelFilter->handleWindow(channel1.first, channel1.second);
    mChannelFilter->handleWindow(channel2.first, channel2.second);
}

}
