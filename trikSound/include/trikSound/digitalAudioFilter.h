#pragma once

#include <memory>
#include <iterator>

#include "triksound_global.h"
#include "audioFilter.h"
#include "digitalAudioFilterImpl.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT DigitalAudioFilter : public AudioFilter<Iter>
{
public:

    typedef typename AudioFilter<Iter>::FilterPtr FilterPtr;

    DigitalAudioFilter();

private:
    void handleWindowImpl(Iter first, Iter last);

    std::unique_ptr<DigitalAudioFilterImpl<Iter>> mImpl;
};

template <typename Iter>
DigitalAudioFilter<Iter>::DigitalAudioFilter():
    mImpl(new DigitalAudioFilterImpl<Iter>())
{}

template <typename Iter>
void DigitalAudioFilter<Iter>::handleWindowImpl(Iter first, Iter last)
{
    mImpl->handleWindowImpl(first, last, typename std::iterator_traits<Iter>::iterator_category());
}

}
