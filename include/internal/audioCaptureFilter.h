#ifndef AUDIOCAPTUREFILTER_H
#define AUDIOCAPTUREFILTER_H

#include <QObject>
#include <QSharedPointer>

#include "iAudioFilter.h"
#include "audioDeviceManager.h"

class AudioCaptureFilter: public QObject, public IAudioFilter
{
public:
    AudioCaptureFilter(const QSharedPointer<AudioDeviceManager>& deviceManager, quint16 frameLength);

private:
    QSharedPointer<AudioDeviceManager> mDeviceManager;
    quint16 mFrameLength;
};

#endif // AUDIOCAPTUREFILTER_H
