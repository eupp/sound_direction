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

#include <iterator>
#include <array>

#include <QtGlobal>
#include <QAudioFormat>

namespace trikSound {

inline qint64 samplesForDuration(qint64 duration, const QAudioFormat& format)
{
    return (duration * format.sampleRate() * format.channelCount()) / 1000;
}

inline qint64 bytesForDuration(qint64 duration, const QAudioFormat& format)
{
    return (duration * format.sampleRate() * (format.sampleSize() / 8) * format.channelCount()) / 1000;
}

template <int channelsCount, typename InputIter, typename OutputIter>
void interweaveChannels(std::array<std::pair<InputIter, InputIter>, channelsCount> channels, OutputIter dst)
{
    typedef typename std::iterator_traits<InputIter>::value_type value_type;
    bool rangeEndFlag = false;
    std::array<value_type, channelsCount> buf;
    while (!rangeEndFlag) {
        for (int i = 0; i < channelsCount; ++i) {
            auto& it = channels[i].first;
            if (it == channels[i].second) {
                rangeEndFlag = true;
                break;
            }
            buf[i] = *it;
            ++it;
        }
        if (!rangeEndFlag) {
            std::copy(buf.begin(), buf.end(), dst);
            dst += channelsCount;
        }
    }
}

/**
 * Extract channel from buffer with interweave channels to separate buffer.
 */
template <int channelsCount, int channelNum, typename InputIter, typename OutputIter>
void extractChannel(InputIter first, InputIter last, OutputIter dst)
{
    int n = std::distance(first, last) / channelsCount;
    for (int i = 0; i < n; ++i) {
        *dst = first[i * channelsCount + channelNum];
        ++dst;
    }
}

/**
 * Extract channel from buffer with interweave channels to separate buffer.
 *
 * @param src Source buffer
 * @param dst Destination buffer. Must have size equal to (count / channelCount)
 * @param count Size of src buffer
 */
template <int bytesPerSample, int channelsCount, int channelNum>
void extractChannel(const char* src, char* dst, size_t count)
{
    typedef std::array<char, bytesPerSample> sampleBytes;
    const sampleBytes* src_ = reinterpret_cast<const sampleBytes*>(src);
    sampleBytes* dst_ = reinterpret_cast<sampleBytes*>(dst);
    extractChannel<channelsCount, channelNum>(src_, src_ + count, dst_);
}

}
