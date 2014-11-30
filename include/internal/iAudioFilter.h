#ifndef AUDIOFILTER_H
#define AUDIOFILTER_H

#include <QObject>

#include "audioBuffer.h"

class IAudioFilter
{
public:
    virtual ~IAudioFilter() {}

signals:
    virtual void output(const AudioBuffer& buf) = 0;
public slots:
    virtual void input(const AudioBuffer& buf) = 0;
};

#endif // AUDIOFILTER_H
