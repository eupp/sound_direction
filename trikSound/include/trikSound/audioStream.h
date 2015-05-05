#pragma once

#include <QObject>

#include "triksound_global.h"
#include "types.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioStream: public QObject
{
    Q_OBJECT
public:

    AudioStream(QObject* parent = 0):
        QObject(parent)
    {}
    virtual ~AudioStream() {}

    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void read(sample_type* buf) = 0;
    // count of samples available for reading per channels (i.e. channel[i].samplesAvailable() )
    virtual size_t samplesAvailable() const = 0;

    virtual size_t windowSize() const = 0;
    virtual void setWindowSize(size_t size) = 0;

signals:

    virtual void readyRead();
};

}
