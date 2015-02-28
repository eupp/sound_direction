#pragma once

#include <QObject>
#include <QSharedPointer>

#include "triksound_global.h"
#include "iAudioFilter.h"
#include "digitalAudioFilter.h"
#include "thresholdVadFilter.h"
#include "angleDetector.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT AngleFilter: public QObject, public IAudioFilter
{
    Q_OBJECT
public:
    AngleFilter(double micrDist, double threshold);
signals:
    void output(AudioBuffer buf);
public slots:
    AudioBuffer input(const AudioBuffer &buf);
private:
    QSharedPointer<IAudioFilter> mFilter;
    ThresholdVadFilter mVad;
    AngleDetector mDetector;
    double mMicrDist;
};

}

