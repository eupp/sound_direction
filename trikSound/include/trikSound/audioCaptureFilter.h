#pragma once

#include <QObject>
#include <QSharedPointer>

#include "triksound_global.h"
#include "iAudioFilter.h"
#include "audioDeviceManager.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT AudioCaptureFilter: public QObject, public IAudioFilter
{
    Q_OBJECT
public:
    AudioCaptureFilter(const std::shared_ptr<AudioDeviceManager>& deviceManager,
                       size_t frameLength,
                       QObject* parent = 0);

signals:
    void output(AudioBuffer buf);
public slots:
    AudioBuffer input(const AudioBuffer &buf);
private slots:
    void bufferReadyReadHandler();
private:
    std::shared_ptr<AudioDeviceManager> mDeviceManager;
    std::shared_ptr<QIODevice> mBuffer;
    quint16 mFrameLength;
};

}
