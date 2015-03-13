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

    typedef typename StereoAudioFilter<Iter>::range_type range_type;
    typedef typename AudioFilter<Iter>::FilterPtr FilterPtr;
    typedef typename StereoAudioFilter<Iter>::FilterPtr StereoFilterPtr;

    SplitFilter(const FilterPtr& channelFilter = FilterPtr(),
                const StereoFilterPtr& prevFilter = StereoFilterPtr());

protected:

    void handleWindowImpl(range_type channel1,
                          range_type channel2);

private:
    FilterPtr mChannelFilter;
};

template <typename Iter>
SplitFilter<Iter>::SplitFilter(const SplitFilter<Iter>::FilterPtr& channelFilter,
                               const SplitFilter<Iter>::StereoFilterPtr& prevFilter):
    StereoAudioFilter<Iter>(prevFilter)
  , mChannelFilter(channelFilter)
{}

template <typename Iter>
void SplitFilter<Iter>::handleWindowImpl(range_type channel1,
                                         range_type channel2)
{
    if (mChannelFilter) {
        mChannelFilter->handleWindow(channel1.first, channel1.second);
        mChannelFilter->handleWindow(channel2.first, channel2.second);
    }
}

}
