#include "include/internal/audioDeviceManager.h"

AudioDeviceManager::AudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                                       const QAudioFormat& audioFormat,
                                       quint16 bufCapacity):
    mDeviceInfo(deviceInfo)
  , mInput(deviceInfo, audioFormat)
  , mBuffer(new CircularBuffer(bufCapacity))
{}

QAudioDeviceInfo AudioDeviceManager::deviceInfo() const
{
    return mDeviceInfo;
}

QAudioFormat AudioDeviceManager::audioFormat() const
{
    return mInput.format();
}

QSharedPointer<QIODevice> AudioDeviceManager::buffer() const
{
    return mBuffer;
}

quint16 AudioDeviceManager::bufferCapacity() const
{
    return mBuffer->capacity();
}

void AudioDeviceManager::setBufferCapacity(quint16 capacity)
{
    mBuffer->setCapacity(capacity);
}

void AudioDeviceManager::start()
{
    mInput.start(mBuffer.data());
}

void AudioDeviceManager::stop()
{
    mInput.stop();
}

void AudioDeviceManager::suspend()
{
    mInput.suspend();
}

void AudioDeviceManager::resume()
{
    mInput.resume();
}


