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


