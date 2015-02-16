#include "include/internal/digitalAudioFilter.h"

#include <algorithm>

#include "include/internal/realTypeTraits.h"
#include "include/internal/debugUtils.h"


DigitalAudioFilter::DigitalAudioFilter():
    // coeffs of filter are hardcoded
    // to do: multiple coeff to some constant to reduce rounding error
    filter_num({0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281})
  , filter_denum({1.0000, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999})
{}


template <typename filter_T, typename in_T, typename out_T>
void filter(filter_T* b, size_t nb,
            filter_T* a, size_t na,
            const in_T* x, out_T* y, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        y[i] = 0;

        size_t j_to = std::min<int>(i, nb - 1);
        for (size_t j = 0; j <= j_to; j++) {
            y[i] += b[j] * x[i - j];
        }

        j_to = std::min<int>(i, na - 1);
        for (size_t j = 1; j <= j_to; j++) {
            y[i] -= a[j] * y[i - j];
        }
    }
}

template <typename T>
void reverse(T* x, T* y, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        y[n - i - 1] = x[i];
    }
}

void filtfilt(real_t* b, size_t nb,
              real_t* a, size_t na,
              const sample_t* x, sample_t* y, size_t n)
{
    // to do: reduce count of copies
    real_t* y_h1 = new real_t[n];
    real_t* y_h2 = new real_t[n];
    filter(b, nb, a, na, x, y_h1, n);
    reverse(y_h1, y_h2, n);
    filter(b, nb, a, na, y_h2, y_h1, n);

    for (size_t i = 0; i < n; i++) {
        y[n - i - 1] = RealTypeTraits<real_t>::toInt16(y_h1[i]);
    }

    delete[] y_h1;
    delete[] y_h2;
}

AudioBuffer DigitalAudioFilter::input(const AudioBuffer& buf)
{
    // to do: check is buffer in mono/stereo format

    QByteArray bytes(buf.size(), '\0');
    filtfilt(filter_num.data(), filter_num.size(),
             filter_denum.data(), filter_denum.size(),
             (const sample_t*) buf.data(), (sample_t*) bytes.data(), buf.sampleCount());
    AudioBuffer outBuf(bytes, buf.audioFormat());
    emit output(outBuf);
    return outBuf;
}

