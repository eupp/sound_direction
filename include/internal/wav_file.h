#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <stdlib.h>

#include "types.h"

void read_wav(const char* filename, sample_t** buf, size_t* size);

// split 2-th channel wav buffer to two separate buffers
// size - size of input buffer
void split_channels(sample_t* buf, sample_t* ch1, sample_t* ch2, size_t size);

#endif // WAV_FILE_H
