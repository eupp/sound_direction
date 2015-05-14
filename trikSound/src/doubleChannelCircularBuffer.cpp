#include "doubleChannelCircularBuffer.h"

#include <algorithm>
#include <utility>

#include <QDebug>

#include "utils.h"

namespace trikSound
{

DoubleChannelCircularBuffer::DoubleChannelCircularBuffer(size_t capacity):
    mLeftBuffer(capacity / 2)
  , mRightBuffer(capacity / 2)
  , mLeftReadItr(mLeftBuffer.begin())
  , mRightReadItr(mRightBuffer.begin())
  , mLeftWriteItr(back_inserter(mLeftBuffer))
  , mRightWriteItr(back_inserter(mRightBuffer))
{}

void DoubleChannelCircularBuffer::read(trikSound::sample_type* buf, size_t size)
{
    size_t halfSize = size / 2;
    auto leftReadEnd = mLeftReadItr + halfSize;
    auto rightReadEnd = mRightReadItr + halfSize;

    std::copy(mLeftReadItr, leftReadEnd, buf);
    std::copy(mRightReadItr, rightReadEnd, buf + halfSize);

    mLeftReadItr = leftReadEnd;
    mRightReadItr = rightReadEnd;
    if (leftReadEnd == mLeftBuffer.end()) {
        --mLeftReadItr;
        --mRightReadItr;
    }
//    else {
//        mLeftReadItr = mLeftBuffer.begin();
//        mRightReadItr = mRightBuffer.begin();
//    }
}

void DoubleChannelCircularBuffer::write(const sample_type* buf, size_t size)
{
    size_t halfSize = size / 2;

    bool overwriteFlag = false;
    int freeSpace = (mLeftReadItr - mLeftBuffer.begin()) + (mLeftBuffer.capacity() - mLeftBuffer.size());
    if (halfSize > freeSpace) {
        overwriteFlag = true;
    }

    // special case for writing to the empty container
    // in that case mReadItr == cb.begin() == cb.end()
    bool emptyFlag = mLeftBuffer.empty();

    extractChannel<2, 0>(buf, buf + size, mLeftWriteItr);
    extractChannel<2, 1>(buf, buf + size, mRightWriteItr);

    // redirect read iterator to begin in case of overwriting or empty buffer
    if (overwriteFlag || emptyFlag) {
        mLeftReadItr = mLeftBuffer.begin();
        mRightReadItr = mRightBuffer.begin();
    }
}

size_t DoubleChannelCircularBuffer::size() const
{
    return 2 * mLeftBuffer.size();
}

size_t DoubleChannelCircularBuffer::samplesAvailable() const
{
    return 2 * (mLeftBuffer.end() - mLeftReadItr);
}

void DoubleChannelCircularBuffer::resize(size_t size)
{
    DoubleChannelCircularBuffer(size).swap(*this);
}

void DoubleChannelCircularBuffer::clear()
{
    DoubleChannelCircularBuffer(mLeftBuffer.capacity()).swap(*this);
}

int DoubleChannelCircularBuffer::channelCount() const
{
    return 2;
}

void DoubleChannelCircularBuffer::swap(DoubleChannelCircularBuffer& other)
{
    std::swap(mLeftBuffer, other.mLeftBuffer);
    std::swap(mRightBuffer, other.mRightBuffer);
    std::swap(mLeftReadItr, other.mLeftReadItr);
    std::swap(mLeftWriteItr, other.mLeftWriteItr);
    std::swap(mRightReadItr, other.mRightReadItr);
    std::swap(mRightWriteItr, other.mRightWriteItr);
}

}
