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
