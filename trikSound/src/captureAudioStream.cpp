#include "captureAudioStream.h"

namespace trikSound {

CaptureAudioStream::CaptureAudioStream(CaptureAudioStream::AudioDeviceManagerPtr device,
                                       CaptureAudioStream::CircularBufferQAdapterPtr buffer,
                                       size_t windowSize,
                                       QObject* parent):
    AudioStream(parent)
  , mDevice(device)
  , mBuffer(buffer)
  , mWindowSize(windowSize)
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

void CaptureAudioStream::read(sample_type* buf)
{
    mBuffer->read((char*)buf, mWindowSize * sizeof(sample_type));
}

size_t CaptureAudioStream::samplesAvailable() const
{
    return mBuffer->samplesAvailable() / mBuffer->channelCount();
}

size_t CaptureAudioStream::windowSize() const
{
    return mWindowSize;
}

void CaptureAudioStream::setWindowSize(size_t size)
{
    mWindowSize = size;
}

}

