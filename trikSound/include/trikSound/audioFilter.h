#pragma once

#include <memory>
#include <utility>

#include "triksound_global.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AudioFilter
{
public:

    typedef std::pair<Iter, Iter> range_type;

    typedef std::shared_ptr<AudioFilter<Iter>> ptrFilter;

    AudioFilter(const ptrFilter& prevFilter = ptrFilter());
    virtual ~AudioFilter() {}

    void handleWindow(Iter first, Iter last);

protected:

    virtual void handleWindowImpl(Iter first, Iter last) = 0;

private:
    std::shared_ptr<AudioFilter<Iter>> mPrev;
};

template <typename Iter>
AudioFilter<Iter>::AudioFilter(const ptrFilter& prevFilter):
    mPrev(prevFilter)
{}

template <typename Iter>
void AudioFilter<Iter>::handleWindow(Iter first, Iter last)
{
    if (mPrev) {
        mPrev->handleWindow(first, last);
    }
    handleWindowImpl(first, last);
}

}
