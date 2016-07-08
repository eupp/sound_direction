/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
