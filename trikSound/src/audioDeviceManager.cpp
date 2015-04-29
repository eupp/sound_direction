#include "audioDeviceManager.h"

#include <QDebug>

using namespace trikSound;

AudioDeviceManager::AudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                                       const QAudioFormat& audioFormat,
                                       const std::shared_ptr<QIODevice>& buffer):
    mDeviceInfo(deviceInfo)
  , mInput(deviceInfo, audioFormat)
  , mBuffer(buffer)
{
    if (!mBuffer->isWritable()) {
        throw InitException("AudioDeviceManager error. Buffer is not writable");
    }
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

void AudioDeviceManager::setBuffer(const std::shared_ptr<QIODevice>& buffer)
{
    stop();
    mBuffer = buffer;
}

void AudioDeviceManager::start()
{
//    qDebug() << "Start capture";
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


