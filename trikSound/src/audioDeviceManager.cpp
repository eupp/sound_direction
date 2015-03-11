#include "audioDeviceManager.h"

using namespace trikSound;

AudioDeviceManager::AudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                                       const QAudioFormat& audioFormat,
                                       const std::shared_ptr<QIODevice>& buffer):
    mDeviceInfo(deviceInfo)
  , mInput(deviceInfo, audioFormat)
  , mBuffer(buffer)
{
    mBuffer->open(QIODevice::WriteOnly);
}

QAudioDeviceInfo AudioDeviceManager::deviceInfo() const
{
    return mDeviceInfo;
}

QAudioFormat AudioDeviceManager::audioFormat() const
{
    return mInput.format();
}

std::shared_ptr<QIODevice> AudioDeviceManager::buffer() const
{
    return mBuffer;
}

void AudioDeviceManager::start()
{
    mInput.start(mBuffer.get());
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


