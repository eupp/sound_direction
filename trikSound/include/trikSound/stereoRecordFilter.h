#pragma once

#include <memory>
#include <array>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "recordFilter.h"
#include "utils.h"


namespace trikSound
{

template <typename Iter>
class StereoRecordFilter: public StereoAudioFilter<Iter>

{
public:

    using StereoAudioFilter<Iter>::FilterPtr;
    using StereoAudioFilter<Iter>::range_type;

    StereoRecordFilter(const QString& filename, const FilterPtr &prevFilter);

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:

    RecordFilter mRecordFilter;
};

template <typename Iter>
StereoRecordFilter::StereoRecordFilter(const QString& filename,
                                       const StereoAudioFilter::FilterPtr& prevFilter):
    StereoAudioFilter(prevFilter)
  , mRecordFilter(filename)
{}

template <typename Iter>
void StereoRecordFilter<Iter>::handleWindowImpl(range_type channel1,
                                                range_type channel2)
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    std::array<range_type, 2> channels;
    int size = std::distance(channel1.first, channel1.second) + std::distance(channel2.first, channel2.second);
    std::vector<value_type> buf(size);
    channels[0] = channel1;
    channels[1] = channel2;
    interweaveChannels<2>(channels, buf.begin());
    mRecordFilter.handleWindow(buf.begin(), buf.end());
}

}
