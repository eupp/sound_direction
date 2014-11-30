#ifndef AUDIODEVICEMANAGER_H
#define AUDIODEVICEMANAGER_H

#include <QObject>
#include <QAudioInput>
#include <QSharedPointer>

#include "include/internal/trikSoundException.h"
#include "include/internal/circularBuffer.h"

class AudioDeviceManager
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

    AudioDeviceManager(const QAudioDeviceInfo& deviceInfo, const QAudioFormat& audioFormat, quint16 bufCapacity);
    virtual ~AudioDeviceManager() {}

    QAudioDeviceInfo deviceInfo() const;
    QAudioFormat audioFormat() const;
    QSharedPointer<QIODevice> buffer() const;

    virtual double volume() const = 0;
    virtual void setVolume(double vol) = 0;

    quint16 bufferCapacity() const;
    void setBufferCapacity(quint16 capacity);

    void start();
    void stop();
    void suspend();
    void resume();

private:
    QAudioDeviceInfo mDeviceInfo;
    QAudioInput mInput;
    QSharedPointer<CircularBuffer> mBuffer;
};

#endif // AUDIODEVICEMANAGER_H
