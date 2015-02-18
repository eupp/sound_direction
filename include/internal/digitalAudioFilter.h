#ifndef AUDIO_FILTER_H
#define AUDIO_FILTER_H

#include <memory>

#include "include/internal/DigitalAudioFilterImpl.h"

template <typename C>
class DigitalAudioFilter
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


#endif // FILTER_H
