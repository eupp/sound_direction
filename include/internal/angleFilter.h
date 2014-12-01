#ifndef ANGLEFILTER_H
#define ANGLEFILTER_H

#include <QObject>
#include <QSharedPointer>

#include "include/internal/iAudioFilter.h"
#include "include/internal/digitalAudioFilter.h"
#include "include/internal/thresholdVadFilter.h"
#include "include/internal/angleDetector.h"

class AngleFilter: public QObject, public IAudioFilter
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

#endif // ANGLEFILTER_H
