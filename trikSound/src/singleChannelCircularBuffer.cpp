#include "singleChannelCircularBuffer.h"

#include <utility>


namespace trikSound
{

SingleChannelCircularBuffer::SingleChannelCircularBuffer(size_t capacity):
    mBuffer(capacity)
  , mReadItr(mBuffer.begin())
  , mWriteItr(back_inserter(mBuffer))
{}

void SingleChannelCircularBuffer::read(sample_type* buf, size_t size)
{
    auto readEnd = mReadItr + size;
    copy(mReadItr, readEnd, buf);

    mReadItr = readEnd;
    if (readEnd == mBuffer.end()) {
        --mReadItr;
    }
//    else {
//        mReadItr = mBuffer.begin();
//    }
}

void SingleChannelCircularBuffer::write(const sample_type* buf, size_t size)
{
    bool overwriteFlag = false;
    int freeSpace = (mReadItr - mBuffer.begin()) + (mBuffer.capacity() - mBuffer.size());
    if (size > freeSpace) {
        overwriteFlag = true;
    }

    // special case for writing to the empty container
    // in that case mReadItr == cb.begin() == cb.end()
    bool emptyFlag = mBuffer.empty();

    copy(buf, buf + size, mWriteItr);

    // redirect read iterator to begin in case of overwriting or empty buffer
    if (overwriteFlag || emptyFlag) {
        mReadItr = mBuffer.begin();
    }
}

size_t SingleChannelCircularBuffer::size() const
{
    return mBuffer.size();
}

size_t SingleChannelCircularBuffer::samplesAvailable() const
{
    return (mBuffer.end() - mReadItr);
}

void SingleChannelCircularBuffer::resize(size_t size)
{
    SingleChannelCircularBuffer(size).swap(*this);
}

void SingleChannelCircularBuffer::clear()
{
    SingleChannelCircularBuffer(mBuffer.capacity()).swap(*this);
}

int SingleChannelCircularBuffer::channelCount() const
{
    return 1;
}

void SingleChannelCircularBuffer::swap(SingleChannelCircularBuffer& other)
{
    std::swap(mBuffer, other.mBuffer);
    std::swap(mReadItr, other.mReadItr);
    std::swap(mWriteItr, other.mWriteItr);
}

}
