#include "captureAudioStream.h"

namespace trikSound {

CaptureAudioStream::CaptureAudioStream(CaptureAudioStream::AudioDeviceManagerPtr device,
                                       CaptureAudioStream::CircularBufferQAdapterPtr buffer,
                                       QObject* parent):
    AudioStream(parent)
  , mDevice(device)
  , mBuffer(buffer)
{
    mDevice->setBuffer(buffer);
}

void CaptureAudioStream::run()
{
    connect(mBuffer.get(), SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    mDevice->start();
}

void CaptureAudioStream::stop()
{
    mDevice->stop();
    disconnect(mBuffer.get(), 0, this, 0);
}

void CaptureAudioStream::read(sample_type* buf, size_t size)
{
    mBuffer->read((char*)buf, size * sizeof(sample_type));
}

size_t CaptureAudioStream::samplesAvailable() const
{
    return mBuffer->samplesAvailable() / mBuffer->channelCount();
}

}

