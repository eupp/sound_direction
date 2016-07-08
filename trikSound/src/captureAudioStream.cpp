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

