#include "include/internal/angleDetector.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <map>
#include <utility>

#include <QDebug>

#include "include/internal/utils.h"
#include "include/internal/debugUtils.h"

AngleDetector::AngleDetector():
    mImpl(new AngleDetectorImpl())
{}

double AngleDetector::getAngle(const AudioBuffer& signal1, const AudioBuffer& signal2, double micrDist)
{
    if ((signal1.sampleCount() != signal2.sampleCount())
            || (signal1.audioFormat() != signal2.audioFormat())) {
        throw IncorrectSignals();
    }

//    int peak = conv_peak((const sample_t*) signal1.data(), (const sample_t*) signal2.data(), signal1.sampleCount());

//    int fs = signal1.audioFormat().sampleRate();

//    double a = (double(peak) * 33000) / (2 * fs);
//    double c = micrDist;
//    double pi = 3.14159265359;

//    double phi = 0;
//    if (std::abs(a) > std::abs(c)) {
//        // sign(a) * pi/2
//        phi = ((a > 0) - (a < 0)) * pi/2;
//    }
//    else {
//        phi = pi/2 - acos(a/c);
//    }

//    return (phi * 180) / pi;
}
