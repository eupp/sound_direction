#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>

#include "include/internal/math_utils.h"
#include "include/internal/types.h"
#include "include/internal/wav_file.h"

using namespace std;

typedef double real;

template <typename T>
void debug_print(const char* filename, T* array, size_t size)
{
    ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file " << filename << endl;
        return;
    }
    for (size_t i = 0; i < size; i++) {
        file << array[i] << endl;
    }
}

int main(void)
{
    const char* filename = "center.wav";

    const size_t offset  = 2000;

    size_t size   = 0;
    sample_t* buf = NULL;

    read_wav(filename, &buf, &size);

    // length of each channels after cutting file
    size_t len = (size - offset) / 2;

    debug_print<sample_t>("wav.test", buf, size);

    sample_t* ch1 = new sample_t[len];
    sample_t* ch2 = new sample_t[len];

    split_channels(buf + offset, ch1, ch2, size - offset);

    debug_print<sample_t>("ch1.test", ch1, len);
    debug_print<sample_t>("ch2.test", ch2, len);

    // filter coeffs



    real af[] = {1.0000, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999};
    real bf[] = {0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281};

    //cout << sizeof(b) / sizeof(b[0]) << " " << sizeof(a) / sizeof(a[0]) << endl;

    real* sig1 = new real[len];
    real* sig2 = new real[len];

    int N = 30;
    sample_t test[N];
    for (int i = 1; i <= N; i++) {
        test[i - 1] = i;
    }
    real test_f[N];

//    filter<real, sample_t, real>(b, 7,
//                                a, 7,
//                                test, test_f, N);

//    filter<real, sample_t, real>(b, sizeof(b) / sizeof(b[0]),
//                                a, sizeof(a) / sizeof(a[0]),
//                                test, test_f, N);

    filtfilt<real, sample_t, real>(bf, sizeof(bf) / sizeof(bf[0]),
                                   af, sizeof(af) / sizeof(af[0]),
                                   test, test_f, N);

    filtfilt<real, sample_t, real>(bf, sizeof(bf) / sizeof(bf[0]),
                                   af, sizeof(af) / sizeof(af[0]),
                                   ch1, sig1, len);

    filtfilt<real, sample_t, real>(bf, sizeof(bf) / sizeof(bf[0]),
                                   af, sizeof(af) / sizeof(af[0]),
                                   ch2, sig2, len);

//    debug_print("filt1.test", sig1, len);
//    debug_print("filt2.test", sig2, len);
//    debug_print("filt.test", test_f, N);

    size_t L = 40;
    size_t conv_len = 2 * L + 1;
    real* conv = new real[conv_len];
    for (int i = -L; i <= 0; i++) {
        conv[i + L]  = dot_product(sig2, sig1 - i, len + i);
    }
    for (int i = 1; i <= L; i++) {
        conv[i + L] = dot_product(sig2 + i, sig1, len - i);
    }

    debug_print("conv.test", conv, conv_len);

    real* max = max_element(conv, conv + conv_len);
    int d = max - conv;
    d -= L;

    cout << d << endl;

    int fs = 44100;
    real c = 10.2;
    real a = ((real)d * 33000) / (2 * fs);

    cout << a << endl;

    real pi = 3.14159265359;
    real phi = 0;
    if (abs(a) > abs(c)) {
        phi = ((a > 0) - (a < 0)) * pi/2;
    }
    else {
        phi = pi/2 - acos(a/c);
    }

    cout << (phi * 180) / pi << endl;

    return 0;
}

