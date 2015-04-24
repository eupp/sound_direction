#pragma once

#include <boost/circular_buffer.hpp>

#include "circularBuffer.h"

namespace trikSound
{

class DoubleChannelCircularBuffer: public CircularBuffer
{
public:

    explicit DoubleChannelCircularBuffer(size_t capacity);

    void read(sample_type* buf, size_t size);
    void write(const sample_type* buf, size_t size);

    size_t size() const;
    size_t samplesAvailable() const;

    void resize(size_t size);
    void clear();

    int channelCount() const;

    void swap(DoubleChannelCircularBuffer& other);

private:

    typedef boost::circular_buffer<sample_type>     BoostBuffer;

    typedef BoostBuffer::const_iterator             ReadIterator;
    typedef std::back_insert_iterator<BoostBuffer>  WriteIterator;

    BoostBuffer         mLeftBuffer;
    BoostBuffer         mRightBuffer;

    ReadIterator        mLeftReadItr;
    ReadIterator        mRightReadItr;
    WriteIterator       mLeftWriteItr;
    WriteIterator       mRightWriteItr;

};

}
