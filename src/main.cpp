#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

#include "include/internal/math_utils.h"
#include "include/internal/types.h"
#include "include/internal/wav_file.h"

using namespace std;
using namespace fpml;

template<typename BB, unsigned char II, unsigned char FF>
fpml::fixed_point<BB, II, FF> abs(fpml::fixed_point<BB, II, FF> val)
{
    return fabs(val);
}

inline double to_double(double val)
{
    return val;
}

template<typename BB, unsigned char II, unsigned char FF>
inline double to_double(fpml::fixed_point<BB, II, FF> val)
{
    return val.to_double();
}

struct Arguments
{
    char const *     filename;
    real_t          micr_dist;
};

enum CLIError
{
    OK,
    FEW_ARGS,
    FILE_NOT_SPECIFIED,
    DIST_NOT_SPECIFIED
};

Arguments parse_args(int argc, char** argv, CLIError* err)
{
    Arguments args = {nullptr, real_t(0)};
    bool filename_set = false;
    bool m_dist_set   = false;
    *err = OK;
    if (argc <= 1) {
        *err = FEW_ARGS;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i] == string("-f")) {
            if (++i >= argc) {
                *err = FILE_NOT_SPECIFIED;
            }
            args.filename = argv[i];
            filename_set = true;
        }
        else if (argv[i] == string("-c")) {
            if (++i >= argc) {
                *err = DIST_NOT_SPECIFIED;
            }
            args.micr_dist = real_t(atof(argv[i]));
            m_dist_set = true;
        }
    }
    if (!filename_set) {
        *err = FILE_NOT_SPECIFIED;
    }
    if (!m_dist_set) {
        *err = DIST_NOT_SPECIFIED;
    }

    return args;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(0);

    CLIError cli_err = OK;
    Arguments args = parse_args(argc, argv, &cli_err);

    if (cli_err != OK) {
        if (cli_err == FEW_ARGS) {
            cout << "Too few arguments. Specify filename and distance between microphones" << endl;
            cout << "Usage: sound_direction -f file.wav -c 10.2" << endl;
        }
        if (cli_err == FILE_NOT_SPECIFIED) {
            cout << "Specify filename" << endl;
            cout << "Usage: sound_direction -f file.wav -c 10.2" << endl;
        }
        if (cli_err == DIST_NOT_SPECIFIED) {
            cout << "Specify distance between microphones" << endl;
            cout << "Usage: sound_direction -f file.wav -c 10.2" << endl;
        }
        return 0;
    }

    size_t size   = 0;
    sample_t* buf = nullptr;
    WavHeader header;
    WavReadError wav_err = NO_ERROR;
    read_wav(args.filename, &buf, &size, &header, &wav_err);

    if (wav_err != NO_ERROR) {
        if (wav_err == CANNOT_OPEN_FILE) {
            cout << "Cannot open wav file" << endl;
        }
        if (wav_err == INCORRECT_FILE) {
            cout << "Incorrect wav file" << endl;
        }
        if (wav_err == READ_FAIL) {
            cout << "Error during file reading" << endl;
        }

        if (buf != nullptr) {
            delete[] buf;
        }

        return 0;
    }

    const size_t offset  = 2000;

    if (size < offset) {
        cout << "Wav file is too short" << endl;
        return 0;
    }

    // length of each channels after cutting file
    size_t len = (size - offset) / 2;
    sample_t* ch1 = new sample_t[len];
    sample_t* ch2 = new sample_t[len];


    split_channels(buf + offset, ch1, ch2, size - offset);

    debug_print("ch1.test", ch1, len);
    debug_print("ch2.test", ch2, len);

    // filter coeffs
    real_t af[] = {1.0000, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999};
    real_t bf[] = {0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281};

    real_t* sig1 = new real_t[len];
    real_t* sig2 = new real_t[len];

    clock_t c1 = clock();

    filtfilt(bf, sizeof(bf) / sizeof(bf[0]),
             af, sizeof(af) / sizeof(af[0]),
             ch1, sig1, len);

    clock_t c2 = clock();

    cout << "Time #1: " << (c2 - c1) / (CLOCKS_PER_SEC / 1000) << endl;

    c1 = clock();

    filtfilt(bf, sizeof(bf) / sizeof(bf[0]),
             af, sizeof(af) / sizeof(af[0]),
             ch2, sig2, len);

    c2 = clock();

    cout << "Time #2: " << (c2 - c1) / (CLOCKS_PER_SEC / 1000) << endl;

    debug_print("filt1.test", sig1, len);
    debug_print("filt2.test", sig2, len);

//    for (int i = 0; i < len; i++) {
//        ch1[i] = round(sig1[i]);
//        ch2[i] = round(sig2[i]);
//    }


    int fs = header.sampleRate;
    real_t c = args.micr_dist;
    real_t pi = 3.14159265359;

    c1 = clock();

    int d = conv_peak(sig1, sig2, len);

    c2 = clock();

    cout << "Time #3: " << (c2 - c1) / (CLOCKS_PER_SEC / 1000) << endl;

    real_t a = (real_t(d) * 33000) / (2 * fs);

    cout << "d = " << d << "; a = " << a << endl;

    real_t phi = 0;
    if (abs(a) > abs(c)) {
        phi = ((a > real_t(0)) - (a < real_t(0))) * pi/2;
    }
    else {
        phi = pi/2 - real_t(acos(to_double(a/c)));
    }

    cout << "Angle: " << (phi * 180) / pi << endl;


//    cout << "Time #4: " << (c2 - c1) / (CLOCKS_PER_SEC / 1000) << endl;

    delete[] buf;
    delete[] ch1;
    delete[] ch2;
    delete[] sig1;
    delete[] sig2;

    return 0;
}

