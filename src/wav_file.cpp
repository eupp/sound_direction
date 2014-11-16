#include "include/internal/wav_file.h"

#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;

void read_wav(const char* filename, sample_t** buf, size_t* size, WavHeader* header, WavReadError* err)
{
    *buf = nullptr;
    *size = 0;
    *err = NO_ERROR;

    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        *err = CANNOT_OPEN_FILE;
        return;
    }

    assert(sizeof(WavHeader) == 44);
    file.read((char*)header, sizeof(WavHeader));

    if (!file.good()) {
        *err = READ_FAIL;
        return;
    }

    if (file.gcount() < sizeof(WavHeader)) {
        *err = INCORRECT_FILE;
        return;
    }

    size_t length = header->subchunk2Size;
    *size = length / sizeof(sample_t);
    *buf = new sample_t[*size];

    file.read((char*)(*buf), length);

    if (!file.good()) {
        *err = READ_FAIL;
        return;
    }
}


void split_channels(sample_t* buf, sample_t* ch1, sample_t* ch2, size_t size)
{
    for (size_t i = 0; i < size / 2; i++) {
        ch1[i] = buf[2 * i];
        ch2[i] = buf[2 * i + 1];
    }
}
