#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "types.h"

template <typename T>
void debug_print(const char* filename, T* array, size_t size)
{
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }
    for (size_t i = 0; i < size; i++) {
        file << array[i] << std::endl;
    }
}

long long to_longlong(double val)
{
    return (long long) val;
}

template<typename BB, unsigned char II, unsigned char FF>
long long to_longlong(fpml::fixed_point<BB, II, FF> val)
{
    return val.to_longlong();
}

template <typename T>
long long dot_product(T* x, T* y, size_t n)
{
    long long res = 0;
    for (size_t i = 0; i < n; ++i) {
        long long xi = to_longlong(floor(x[i]));
        long long yi = to_longlong(floor(y[i]));
        res += xi * yi;
    }
    return res;
}


template <typename T>
int conv_peak(T* u, T* v, size_t n)
{
    size_t L = 40;
    size_t conv_len = 2 * L + 1;
    long long* conv = new long long[conv_len];

    for (int i = -L; i <= 0; i++) {
        conv[i + L]  = dot_product(v, u - i, n + i);
    }
    for (int i = 1; i <= L; i++) {
        conv[i + L] = dot_product(v + i, u, n - i);
    }

//    debug_print("conv.test", conv, conv_len);

    long long* max = std::max_element(conv, conv + conv_len);
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
