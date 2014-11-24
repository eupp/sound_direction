#ifndef ANGLEDETECTOR_H
#define ANGLEDETECTOR_H

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"

class AngleDetector
{
public:
    class IncorrectSignals {};

    double getAngle(const AudioBuffer& signal1, const AudioBuffer& signal2, double micrDist);
};

#endif // ANGLEDETECTOR_H
