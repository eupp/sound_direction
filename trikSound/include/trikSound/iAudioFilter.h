#pragma once

#include <QObject>

#include "audioBuffer.h"


namespace trikSound {

class TRIKSOUNDSHARED_EXPORT IAudioFilter
{
public:
    virtual ~IAudioFilter() {}

signals:
    virtual void output(AudioBuffer buf) = 0;
public slots:
    virtual AudioBuffer input(const AudioBuffer& buf) = 0;
};

}
