#pragma once

#include <iostream>
#include <fstream>
#include <iterator>

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

/**
 * Extract channel from buffer with interweave channels to separate buffer.
 */
template <int channelCount, int channelNum, typename InputIter, typename OutputIter>
void extractChannel(InputIter first, InputIter last, OutputIter dst)
{
    int n = distance(first, last) / channelCount;
    for (int i = 0; i < n; ++i) {
        dst[i] = first[i * channelCount + channelNum];
    }
}

/**
 * Extract channel from buffer with interweave channels to separate buffer.
 *
 * @param src Source buffer
 * @param dst Destination buffer. Must have size equal to (count / channelCount)
 * @param count Size of src buffer
 */
template <int bytesPerSample, int channelCount, int channelNum>
void extractChannel(const char* src, char* dst, size_t count)
{
    typedef std::array<char, bytesPerSample> sampleBytes;
    const sampleBytes* src_ = reinterpret_cast<const sampleBytes*>(src);
    sampleBytes* dst_ = reinterpret_cast<sampleBytes*>(dst);
    extractChannel<channelCount, channelNum>(src_, src_ + count, dst_);
}

}
