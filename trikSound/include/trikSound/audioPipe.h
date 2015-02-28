#pragma once

#include <QObject>
#include <QSharedPointer>

#include "triksound_global.h"
#include "iAudioFilter.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT AudioPipe: public QObject
{
    Q_OBJECT
public:
    AudioPipe(const QSharedPointer<IAudioFilter>& in,
              const QSharedPointer<IAudioFilter>& out,
              QObject* parent = 0):
        QObject(parent)
      , mIn(in)
      , mOut(out)
    {
        connect(dynamic_cast<QObject*>(mIn.data()), SIGNAL(output(AudioBuffer)),
                dynamic_cast<QObject*>(mOut.data()), SLOT(input(AudioBuffer)));
    }
    ~AudioPipe()
    {
        disconnect(dynamic_cast<QObject*>(mIn.data()), SIGNAL(output(AudioBuffer)),
                   dynamic_cast<QObject*>(mOut.data()), SLOT(input(AudioBuffer)));
    }

private:
    QSharedPointer<IAudioFilter> mIn;
    QSharedPointer<IAudioFilter> mOut;
};

}
