#pragma once

#include "../trikSound/types.h"

namespace trikSound
{

class CircularBuffer
{
public:
    virtual void read(sample_type* buf, size_t size) = 0;
    virtual void write(const sample_type* buf, size_t size) = 0;

    virtual size_t size() const = 0;
    virtual size_t samplesAvailable() const = 0;

    virtual void resize(size_t size) = 0;
    virtual void clear() = 0;

    virtual int channelCount() const = 0;
};


}
