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
    int n = distance(first, last) / channelsCount;
    for (int i = 0; i < n; ++i) {
        dst[i] = first[i * channelsCount + channelNum];
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
