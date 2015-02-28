#pragma once

#include <memory>

#include "triksound_global.h"
#include "digitalAudioFilterImpl.h"

namespace trikSound {

template <typename C>
class TRIKSOUNDSHARED_EXPORT DigitalAudioFilter
{
public:
    DigitalAudioFilter();

    typedef typename DigitalAudioFilterImpl<C>::iterator_type iterator_type;

    void handleWindow(iterator_type first, iterator_type last);

private:
    std::unique_ptr<DigitalAudioFilterImpl<C>> mImpl;
};

template <typename C>
DigitalAudioFilter<C>::DigitalAudioFilter():
    mImpl(new DigitalAudioFilterImpl<C>())
{}

template <typename C>
void DigitalAudioFilter<C>::handleWindow(iterator_type first, iterator_type last)
{
    mImpl->handleWindow(first, last);
}

}
