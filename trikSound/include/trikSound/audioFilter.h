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
    // Set filter as previous filter to this.
    // If current filter already had previous filter, it move it to the tail of chain of filters
    // specified by prevFilter.
    void insertFilter(FilterPtr& prevFilter);

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
void AudioFilter<Iter>::insertFilter(typename AudioFilter::FilterPtr& prev)
{
    if (!prev) {
        return;
    }
    if (prev->prevFilter()) {
        prev->prevFilter()->insertFilter(mPrev);
    }
    else {
        prev->insertFilter(mPrev);
    }
    mPrev = prev;
}


}
