#pragma once

#include <memory>

#include <QAudioFormat>

#include "triksound_global.h"
#include "types.h"
#include "angleDetectorImpl.h"

namespace trikSound {

template <typename C>
class TRIKSOUNDSHARED_EXPORT AngleDetector
{
public:

    typedef typename AngleDetectorImpl<C>::iterator_type iterator_type;

    class IncorrectSignals {};

    AngleDetector(const QAudioFormat& format, double micrDist);

    double getAngle(size_t historyDepth);

    void handleWindow(iterator_type first1, iterator_type last1, iterator_type first2, iterator_type last2);

private:
    std::unique_ptr<AngleDetectorImpl<C>> mImpl;
};

template <typename C>
AngleDetector<C>::AngleDetector(const QAudioFormat& format, double micrDist):
    mImpl(new AngleDetectorImpl<C>(format, micrDist))
{}

template <typename C>
double AngleDetector<C>::getAngle(size_t historyDepth)
{
    return mImpl->getAngle(historyDepth);
}

template <typename C>
void AngleDetector<C>::handleWindow(iterator_type first1,
                                    iterator_type last1,
                                    iterator_type first2,
                                    iterator_type last2)
{
    return mImpl->handleWindow(first1, last1, first2, last2);
}

}
