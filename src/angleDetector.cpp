#include "include/internal/angleDetector.h"

#include <cmath>
#include <algorithm>
#include <numeric>

#include "include/internal/utils.h"

long long dot_product(const sample_t* x, const sample_t* y, size_t n)
{
    long long res = 0;
    for (size_t i = 0; i < n; ++i) {
        res += x[i] * y[i];
    }
    return res;
}

int conv_peak(const sample_t* u, const sample_t* v, size_t n)
{
    const size_t L = 40;
    const size_t frameSize = 2 * L + 1;
    const size_t windowSize = 512;

    long long conv[frameSize];

    n -= frameSize;

    while (windowSize < n) {
        for (size_t i = 0; i <= frameSize; i++) {
//            conv[i] += std::inner_product(v, v + windowSize, u + i, 0ll);
            for (size_t j = 0; j < windowSize; ++j) {
                conv[i] += u[i + j] * v[j];
            }
        }
        v += windowSize;
        u += windowSize;
        n -= windowSize;
    }

    long long* max_conv = std::max_element(conv, conv + frameSize);

    return (max_conv - conv) - L;
}

double AngleDetector::getAngle(const AudioBuffer& signal1, const AudioBuffer& signal2, double micrDist)
{
    if ((signal1.sampleCount() != signal2.sampleCount())
            || (signal1.audioFormat() != signal2.audioFormat())) {
        throw IncorrectSignals();
    }

    int peak = conv_peak((const sample_t*) signal1.data(), (const sample_t*) signal2.data(), signal1.sampleCount());

    int fs = signal1.audioFormat().sampleRate();

    double a = (double(peak) * 33000) / (2 * fs);
    double c = micrDist;
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
