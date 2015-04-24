#pragma once

#include <boost/circular_buffer.hpp>

#include "circularBuffer.h"

namespace trikSound
{

class SingleChannelCircularBuffer: public CircularBuffer
{
public:

    explicit SingleChannelCircularBuffer(size_t capacity);

    void read(sample_type* buf, size_t size);
    void write(const sample_type* buf, size_t size);

    size_t size() const;
    size_t samplesAvailable() const;

    void resize(size_t size);
    void clear();

    int channelCount() const;

    void swap(SingleChannelCircularBuffer& other);

private:

    typedef boost::circular_buffer<sample_type>         BoostBuffer;

    typedef BoostBuffer::const_iterator                 ReadIterator;
    typedef std::back_insert_iterator<BoostBuffer>      WriteIterator;

    BoostBuffer         mBuffer;
    ReadIterator        mReadItr;
    WriteIterator       mWriteItr;
};

}
