#pragma once

#include <QObject>
#include <QAudioInput>
#include <QSharedPointer>

#include "triksound_global.h"
#include "trikSoundException.h"
#include "circularBuffer.h"

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

    AudioDeviceManager(const QAudioDeviceInfo& deviceInfo, const QAudioFormat& audioFormat, size_t bufCapacity);
    virtual ~AudioDeviceManager() {}

    QAudioDeviceInfo deviceInfo() const;
    QAudioFormat audioFormat() const;
    QSharedPointer<QIODevice> buffer() const;

    virtual double volume() const {
        return 1.0;
    }
    virtual void setVolume(double vol) {
        Q_UNUSED(vol);
    }

    size_t bufferCapacity() const;
    void setBufferCapacity(size_t capacity);

    void start();
    void stop();
    void suspend();
    void resume();

private:
    QAudioDeviceInfo mDeviceInfo;
    QAudioInput mInput;
    QSharedPointer<CircularBuffer> mBuffer;
};

}
