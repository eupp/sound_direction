#pragma once

#include <memory>
#include <utility>

#include "audioBuffer.h"


namespace trikSound {


template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AudioFilter
{
public:

    typedef std::pair<Iter, Iter> range_type;

    typedef std::shared_ptr<AudioFilter<Iter>> ptrFilter;

    AudioFilter(const ptrFilter& prevFilter = ptrFilter());
    virtual ~AudioFilter() {}

    range_type handleWindow(Iter first, Iter last);

protected:

    virtual range_type handleWindowImpl(Iter first, Iter last) = 0;

private:
    std::shared_ptr<AudioFilter<Iter>> mPrev;
};

template <typename Iter>
AudioFilter<Iter>::AudioFilter(const ptrFilter& prevFilter):
    mPrev(prevFilter)
{}

template <typename Iter>
typename AudioFilter<Iter>::range_type AudioFilter<Iter>::handleWindow(Iter first, Iter last)
{
    if (mPrev) {
        auto range = mPrev->handleWindow(first, last);
        first = range.first;
        last = range.second;
    }
    return handleWindowImpl(first, last);
}

}
