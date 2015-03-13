#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioFilter.h"
#include "stereoAudioFilter.h"
#include "utils.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT SplitFilter : public StereoAudioFilter<Iter>
{
public:

    SplitFilter(const AudioFilter<Iter>::FilterPtr& channelFilter = AudioFilter<Iter>::FilterPtr(),
                const StereoAudioFilter<Iter>::FilterPtr& prevFilter = StereoAudioFilter<Iter>::FilterPtr());

protected:

    void handleWindowImpl(range_type channel1,
                          range_type channel2);

private:
    AudioFilter::ptrFilter mChannelFilter;
};

template <typename Iter>
SplitFilter::SplitFilter(const AudioFilter<Iter>::FilterPtr& channelFilter,
                         const StereoAudioFilter<Iter>::FilterPtr& prevFilter):
    StereoAudioFilter(prevFilter)
  , mChannelFilter(channelFilter)
{}

template <typename Iter>
void SplitFilter::handleWindowImpl(range_type channel1,
                                   range_type channel2)
{
    mChannelFilter->handleWindow(channel1.first, channel1.second);
    mChannelFilter->handleWindow(channel2.first, channel2.second);
}

}
