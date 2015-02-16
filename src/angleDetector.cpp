#include "include/internal/angleDetector.h"

#include <cmath>
#include <algorithm>

#include "include/internal/utils.h"
#include "include/internal/debugUtils.h"

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
    size_t L = 40;
    size_t conv_len = 2 * L + 1;
    long long* conv = new long long[conv_len];

    // to do: reduce complexity of computations
    // calculate 1 dot product instead of 80 and then update it
    for (int i = -L; i <= 0; i++) {
        conv[i + L]  = dot_product(v, u - i, n + i);
    }
    for (int i = 1; i <= L; i++) {
        conv[i + L] = dot_product(v + i, u, n - i);
    }

    dprint_sequence("conv.test", conv, conv + conv_len);

    long long* max = std::max_element(conv, conv + conv_len);
    int d = max - conv;

    delete[] conv;

    return d - L;
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
