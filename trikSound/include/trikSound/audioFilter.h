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

    AudioFilter();
    virtual ~AudioFilter() {}

    void handleWindow(Iter first, Iter last);

protected:

    virtual void handleWindowImpl(Iter first, Iter last) = 0;

private:
    FilterPtr mPrev;
};

template <typename Iter>
AudioFilter<Iter>::AudioFilter()
{}

template <typename Iter>
void AudioFilter<Iter>::handleWindow(Iter first, Iter last)
{
    handleWindowImpl(first, last);
}

}
