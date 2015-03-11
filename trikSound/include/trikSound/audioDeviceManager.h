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
