#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "include/internal/math_utils.h"
#include "include/internal/types.h"
#include "include/internal/wav_file.h"

using namespace std;

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

    // filter coeffs
    real_t af[] = {1.0000, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999};
    real_t bf[] = {0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281};

    real_t* sig1 = new real_t[len];
    real_t* sig2 = new real_t[len];

    filtfilt(bf, sizeof(bf) / sizeof(bf[0]),
             af, sizeof(af) / sizeof(af[0]),
             ch1, sig1, len);

    filtfilt(bf, sizeof(bf) / sizeof(bf[0]),
             af, sizeof(af) / sizeof(af[0]),
             ch2, sig2, len);


    int fs = header.sampleRate;
    real_t c = args.micr_dist;
    real_t pi = 3.14159265359;
    int d = conv_peak(sig1, sig2, len);

    real_t a = ((real_t)d * 33000) / (2 * fs);

    real_t phi = 0;
    if (abs(a) > abs(c)) {
        phi = ((a > 0) - (a < 0)) * pi/2;
    }
    else {
        phi = pi/2 - acos(a/c);
    }

    cout << "Angle: " << (phi * 180) / pi << endl;

    return 0;
}

