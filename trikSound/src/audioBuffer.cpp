#include "audioBuffer.h"

#include <QDebug>

#include <array>

using namespace trikSound;

AudioBuffer::AudioBuffer(const QByteArray& bytes, const QAudioFormat& format):
    mBytes(bytes)
  , mFormat(format)
{}

QAudioFormat AudioBuffer::audioFormat() const
{
    return mFormat;
}

qint64 AudioBuffer::size() const
{
    return mBytes.size();
}

qint64 AudioBuffer::sampleCount() const
{
    qint64 count = size() / bytesPerSample();
    if (mFormat.channelCount() == 2) {
        count /= 2;
    }
    return count;
}

qint64 AudioBuffer::duration() const
{
    return (sampleCount() * 1000) / mFormat.sampleRate();
}

void* AudioBuffer::data()
{
    return reinterpret_cast<void*>(mBytes.data());
}

const void*AudioBuffer::data() const
{
    return reinterpret_cast<const void*>(mBytes.data());
}

const void*AudioBuffer::constData() const
{
    return reinterpret_cast<const void*>(mBytes.constData());
}

AudioBuffer AudioBuffer::subBuffer(int pos, int len)
{
    return AudioBuffer(mBytes.mid(pos, len), mFormat);
}

template <int bytesPerSample, int chCount, int chNum>
void extractChannel(const char* src, char* dst, size_t count)
{
    typedef std::array<char, bytesPerSample> sampleBytes;
    const sampleBytes* src_ = reinterpret_cast<const sampleBytes*>(src);
    sampleBytes* dst_ = reinterpret_cast<sampleBytes*>(dst);
    for (size_t i = 0; i < count; ++i) {
        dst_[i] = src_[i * chCount + chNum];

//        std::copy(sample, sample + bytesPerSample, dst + i * bytesPerSample);
//        memcpy(dst + i * bytesPerSample, sample, bytesPerSample);

    }
}

AudioBuffer AudioBuffer::leftChannel() const
{
    const size_t count = sampleCount();
    const size_t bytesCount = bytesPerSample();
    const size_t chCount = mFormat.channelCount();
    char* leftChl = new char[size() / chCount];

    if (bytesCount == 2 && chCount == 2) {
        extractChannel<2, 2, 0>(mBytes.data(), leftChl, count);
    }

    QAudioFormat fmt = mFormat;
    fmt.setChannelCount(1);
    QByteArray bytes = QByteArray::fromRawData(leftChl, size() / 2);
    return AudioBuffer(bytes, fmt);
}

AudioBuffer AudioBuffer::rightChannel() const
{
    const size_t count = sampleCount();
    const size_t bytesCount = bytesPerSample();
    const size_t chCount = mFormat.channelCount();
    char* rightChl = new char[size() / chCount];

    if (bytesCount == 2 && chCount == 2) {
        extractChannel<2, 2, 1>(mBytes.data(), rightChl , count);
    }

    QAudioFormat fmt = mFormat;
    fmt.setChannelCount(1);
    QByteArray bytes = QByteArray::fromRawData(rightChl, size() / 2);
    return AudioBuffer(bytes, fmt);
}

int AudioBuffer::bytesPerSample() const
{
    return 2;
//    return mFormat.sampleSize() / 8;
}
