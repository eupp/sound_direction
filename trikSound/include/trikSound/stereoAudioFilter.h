#pragma once

#include <memory>

#include "triksound_global.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT StereoAudioFilter
{
public:

    typedef std::pair<Iter, Iter> range_type;
    typedef std::shared_ptr<StereoAudioFilter<Iter>> FilterPtr;

    static range_type make_empty_range();

    StereoAudioFilter();
    virtual ~StereoAudioFilter() {}

    void handleWindow(range_type channel1, range_type channel2);

protected:
    virtual void handleWindowImpl(range_type channel1, range_type channel2) = 0;

private:
    FilterPtr mPrev;
};

template <typename Iter>
StereoAudioFilter<Iter>::StereoAudioFilter()
{}

template <typename Iter>
void StereoAudioFilter<Iter>::handleWindow(range_type channel1, range_type channel2)
{
    handleWindowImpl(channel1, channel2);
}

template <typename Iter>
typename StereoAudioFilter<Iter>::range_type StereoAudioFilter<Iter>::make_empty_range()
{
    return make_pair(Iter(), Iter());
}

}

