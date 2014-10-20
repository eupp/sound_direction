#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstdlib>
#include <algorithm>

#include "types.h"

template <typename T>
void conv(T* u, size_t un, T* v, size_t uv, T* out)
{
    size_t outn = un + uv - 1;

    for (size_t k = 0; k < outn; k++) {
       size_t j_from = std::max<int>(0, k + 1 - outn);
       size_t j_to   = std::min<int>(un - 1, k);
       out[k] = 0;
       for (size_t j = j_from; j <= j_to; j++) {
           out[k] += u[j] * v[k - j];
       }
    }
}

template <typename filter_T, typename in_T, typename out_T>
void filter(filter_T* b, size_t nb,
            filter_T* a, size_t na,
            in_T* x, out_T* y, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        y[i] = 0;

        size_t j_to = std::min<int>(i, nb - 1);
        for (size_t j = 0; j <= j_to; j++) {
            y[i] += b[j] * x[i - j];
        }

        size_t j_from = std::max<int>(1, i - 1);
        j_to          = std::min<int>(i, na - 1);
        for (size_t j = j_from; j <= j_to; j++) {
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

template <typename filter_T, typename in_T, typename out_T>
void filtfilt(filter_T* b, size_t nb,
              filter_T* a, size_t na,
              in_T* x, out_T* y, size_t n)
{
    out_T* y_h = new out_T[n];
    filter(b, nb, a, na, x, y_h, n);
    reverse(y_h, y, n);
    filter(b, nb, a, na, y, y_h, n);
    reverse(y_h, y, n);
    delete[] y_h;
}

template <typename T>
T dot_product(T* x, T* y, size_t n)
{
    T res = 0;
    for (size_t i = 0; i < n; ++i) {
        res += x[i] * y[i];
    }
    return res;
}

#endif // MATH_UTILS_H
