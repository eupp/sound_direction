#pragma once

#include "triksound_global.h"
#include "iAudioFilter.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT ThresholdVadFilter: public QObject, public IAudioFilter
{
    Q_OBJECT
public:
    ThresholdVadFilter(double threshold);

    double threshold() const;
    void setThreshold(double threshold);

    bool vad(const AudioBuffer& buf);

public slots:
    AudioBuffer input(const AudioBuffer &buf);
signals:
    void output(AudioBuffer buf);
private:
    double mThreshold;
};

}
