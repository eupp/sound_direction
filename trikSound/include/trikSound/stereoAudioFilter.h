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

    StereoAudioFilter(const FilterPtr& prevFilter = FilterPtr());
    virtual ~StereoAudioFilter() {}

    void handleWindow(range_type channel1, range_type channel2);

    FilterPtr prevFilter() const;
    void setPrevFilter(const FilterPtr& prevFilter);

protected:
    virtual void handleWindowImpl(range_type channel1, range_type channel2) = 0;

private:
    FilterPtr mPrev;
};

template <typename Iter>
StereoAudioFilter<Iter>::StereoAudioFilter(const FilterPtr& prevFilter):
    mPrev(prevFilter)
{}

template <typename Iter>
void StereoAudioFilter<Iter>::handleWindow(range_type channel1, range_type channel2)
{
    if (mPrev) {
        mPrev->handleWindow(channel1, channel2);
    }
    handleWindowImpl(channel1, channel2);
}

template <typename Iter>
typename StereoAudioFilter<Iter>::FilterPtr StereoAudioFilter<Iter>::prevFilter() const
{
    return mPrev;
}

template <typename Iter>
void StereoAudioFilter<Iter>::setPrevFilter(const typename StereoAudioFilter::FilterPtr& prev)
{
    mPrev = prev;
}

}
