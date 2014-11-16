#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <cstdlib>
#include <cstdint>

#include "types.h"

struct WavHeader
{
    std::uint8_t    chunkId[4];
    std::uint32_t   chunkSize;
    std::uint8_t    format[4];
    std::uint8_t    subchunk1Id[4];
    std::uint32_t   subchunk1Size;
    std::uint16_t   audioFormat;
    std::uint16_t   numChannels;
    std::uint32_t   sampleRate;
    std::uint32_t   byteRate;
    std::uint16_t   blockAlign;
    std::uint16_t   bitsPerSample;
    std::uint8_t    subchunk2Id[4];
    std::uint32_t   subchunk2Size;
};

enum WavReadError
{
    NO_ERROR,
    INCORRECT_FILE,
    CANNOT_OPEN_FILE,
    READ_FAIL
};

void read_wav(const char* filename, sample_t** buf, size_t* size, WavHeader* header, WavReadError* err);

// split 2-th channel wav buffer to two separate buffers
// size - size of input buffer
void split_channels(sample_t* buf, sample_t* ch1, sample_t* ch2, size_t size);

#endif // WAV_FILE_H
