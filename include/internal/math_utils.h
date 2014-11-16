#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstdlib>
#include <algorithm>

#include "types.h"

template <typename T>
T dot_product(T* x, T* y, size_t n)
{
    T res = 0;
    for (size_t i = 0; i < n; ++i) {
        res += x[i] * y[i];
    }
    return res;
}

template <typename T>
int conv_peak(T* u, T* v, size_t n)
{
    size_t L = 40;
    size_t conv_len = 2 * L + 1;
    real_t* conv = new real_t[conv_len];

    for (int i = -L; i <= 0; i++) {
        conv[i + L]  = dot_product(v, u - i, n + i);
    }
    for (int i = 1; i <= L; i++) {
        conv[i + L] = dot_product(v + i, u, n - i);
    }

    T* max = std::max_element(conv, conv + conv_len);
    int d = max - conv;

    delete[] conv;

    return d - L;
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

#endif // MATH_UTILS_H
