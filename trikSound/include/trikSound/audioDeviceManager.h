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

#include <QAudioInput>

#include "triksound_global.h"
#include "trikSoundException.h"
#include "circularBufferQAdapter.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT AudioDeviceManager
{
public:

    class InitException: public TrikSoundException
    {
    public:
        InitException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    class CommandException: public TrikSoundException
    {
    public:
        CommandException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    AudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                       const QAudioFormat& audioFormat,
                       const std::shared_ptr<QIODevice>& buffer);
    virtual ~AudioDeviceManager() {}

    QAudioDeviceInfo deviceInfo() const;
    QAudioFormat audioFormat() const;
    std::shared_ptr<QIODevice> buffer() const;
    void setBuffer(const std::shared_ptr<QIODevice>& buffer);

    virtual double volume() const {
        return 1.0;
    }
    virtual void setVolume(double vol) {
        Q_UNUSED(vol);
    }

    void start();
    void stop();
    void suspend();
    void resume();

private:
    QAudioDeviceInfo mDeviceInfo;
    QAudioInput mInput;
    std::shared_ptr<QIODevice> mBuffer;
};

}
