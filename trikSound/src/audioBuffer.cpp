#include "audioBuffer.h"

#include <QDebug>

#include <array>

#include "utils.h"

using namespace trikSound;

trikSound::AudioBuffer::AudioBuffer():
    mBytes()
  , mFormat()
{}

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

const void* AudioBuffer::data() const
{
    return reinterpret_cast<const void*>(mBytes.data());
}

const void* AudioBuffer::constData() const
{
    return reinterpret_cast<const void*>(mBytes.constData());
}

AudioBuffer AudioBuffer::subBuffer(int pos, int len)
{
    return AudioBuffer(mBytes.mid(pos, len), mFormat);
}

AudioBuffer AudioBuffer::leftChannel() const
{
    const size_t bytesCount = bytesPerSample();
    const size_t chCount = mFormat.channelCount();
    const size_t count =  chCount * sampleCount();
    char* leftChl = new char[size() / chCount];

    if (bytesCount == 2 && chCount == 2) {
        extractChannel<2, 2, 0>(mBytes.data(), leftChl, count);
    }

    QAudioFormat fmt = mFormat;
    fmt.setChannelCount(1);
    QByteArray bytes = QByteArray::fromRawData(leftChl, size() / chCount);
    return AudioBuffer(bytes, fmt);
}

AudioBuffer AudioBuffer::rightChannel() const
{
    const size_t bytesCount = bytesPerSample();
    const size_t chCount = mFormat.channelCount();
    const size_t count =  chCount * sampleCount();
    char* rightChl = new char[size() / chCount];

    if (bytesCount == 2 && chCount == 2) {
        extractChannel<2, 2, 1>(mBytes.data(), rightChl , count);
    }

    QAudioFormat fmt = mFormat;
    fmt.setChannelCount(1);
    QByteArray bytes = QByteArray::fromRawData(rightChl, size() / chCount);
    return AudioBuffer(bytes, fmt);
}

int AudioBuffer::bytesPerSample() const
{
    return mFormat.sampleSize() / 8;
}



