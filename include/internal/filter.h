#ifndef AUDIO_FILTER_H
#define AUDIO_FILTER_H

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"

class AudioFilter
{
public:
    AudioBuffer filter(const AudioBuffer& buf);
};

#endif // FILTER_H
