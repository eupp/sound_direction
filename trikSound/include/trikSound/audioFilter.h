#pragma once

#include <memory>
#include <utility>

#include "triksound_global.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AudioFilter
{
public:

    typedef std::shared_ptr<AudioFilter<Iter>> FilterPtr;

    AudioFilter(const FilterPtr& prevFilter = FilterPtr());
    virtual ~AudioFilter() {}

    void handleWindow(Iter first, Iter last);

    FilterPtr prevFilter() const;
    void setPrevFilter(const FilterPtr& prevFilter);

protected:

    virtual void handleWindowImpl(Iter first, Iter last) = 0;

private:
    FilterPtr mPrev;
};

template <typename Iter>
AudioFilter<Iter>::AudioFilter(const FilterPtr& prevFilter):
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

template <typename Iter>
typename AudioFilter<Iter>::FilterPtr AudioFilter<Iter>::prevFilter() const
{
    return mPrev;
}

template <typename Iter>
void AudioFilter<Iter>::setPrevFilter(const typename AudioFilter::FilterPtr& prev)
{
    mPrev = prev;
}


}
