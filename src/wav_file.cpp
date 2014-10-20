#include "include/internal/wav_file.h"

#include <iostream>
#include <fstream>

using namespace std;


void read_wav(const char* filename, sample_t** buf, size_t* size)
{
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file " << filename << endl;
        *buf = NULL;
        *size = 0;
        return;
    }

    file.seekg(0, file.end);
    size_t length = file.tellg() - 44;
    file.seekg (44, file.beg);

    *size = length / sizeof(sample_t);
    *buf = new sample_t[*size];

    // read data as a block:
    file.read((char*)(*buf), length);
}


void split_channels(sample_t* buf, sample_t* ch1, sample_t* ch2, size_t size)
{
    for (size_t i = 0; i < size / 2; i++) {
        ch1[i] = buf[2 * i];
        ch2[i] = buf[2 * i + 1];
    }
}
