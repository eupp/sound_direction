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

#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioStream.h"
#include "audioDeviceManager.h"
#include "circularBufferQAdapter.h"


namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT CaptureAudioStream: public AudioStream
{
public:

    typedef std::shared_ptr<AudioDeviceManager>  AudioDeviceManagerPtr;
    typedef std::shared_ptr<CircularBufferQAdapter>  CircularBufferQAdapterPtr;

    CaptureAudioStream(AudioDeviceManagerPtr device,
                       CircularBufferQAdapterPtr buffer,
                       size_t windowSize,
                       QObject* parent = 0);

    void run();
    void restart();
    void stop();
    void read(sample_type *buf);
    size_t samplesAvailable() const;

    size_t windowSize() const;
    void setWindowSize(size_t size);

private:

    AudioDeviceManagerPtr mDevice;
    CircularBufferQAdapterPtr mBuffer;
    size_t mWindowSize;
};

}
