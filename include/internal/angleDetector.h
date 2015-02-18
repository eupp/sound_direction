#ifndef ANGLEDETECTOR_H
#define ANGLEDETECTOR_H

#include <memory>

#include <QAudioFormat>

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/angleDetectorImpl.h"

template <typename C>
class AngleDetector
{
public:

    typedef typename AngleDetectorImpl<C>::iterator_type iterator_type;

    class IncorrectSignals {};

    AngleDetector(const QAudioFormat& format, double micrDist):
        mImpl(new AngleDetectorImpl<C>()),
        mSampleRate(format.sampleRate()),
        mMicrDist(micrDist)
    {}

    double getAngle();

    void handleWindow(iterator_type first1, iterator_type last1, iterator_type first2, iterator_type last2)
    {
        return mImpl->handleWindow(first1, last1, first2, last2);
    }

private:
    std::unique_ptr<AngleDetectorImpl<C>> mImpl;
    const int mSampleRate;
    const int mMicrDist;
};

template <typename C>
double AngleDetector<C>::getAngle()
{
    int fs = mSampleRate;

    int peak = mImpl->getHypothesis(0);

    double a = (double(peak) * 33000) / (2 * fs);
    double c = mMicrDist;
    double pi = 3.14159265359;

    double phi = 0;
    if (std::abs(a) > std::abs(c)) {
        // sign(a) * pi/2
        phi = ((a > 0) - (a < 0)) * pi/2;
    }
    else {
        phi = pi/2 - acos(a/c);
    }

    return (phi * 180) / pi;
}

#endif // ANGLEDETECTOR_H
