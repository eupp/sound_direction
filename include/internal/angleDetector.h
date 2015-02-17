#ifndef ANGLEDETECTOR_H
#define ANGLEDETECTOR_H

#include <memory>

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/angleDetectorImpl.h"

class AngleDetector
{
public:

    class IncorrectSignals {};

    AngleDetector();

    double getAngle(const AudioBuffer& signal1, const AudioBuffer& signal2, double micrDist);

    template <typename T>
    void handleWindow(T first1, T last1, T first2, T last2)
    {
        return mImpl->handleWindow(first1, last1, first2, last2);
    }

private:
    std::unique_ptr<AngleDetectorImpl> mImpl;
};

#endif // ANGLEDETECTOR_H
