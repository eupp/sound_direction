#ifndef AUDIO_FILTER_H
#define AUDIO_FILTER_H

#include <QVector>

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/iAudioFilter.h"

class DigitalAudioFilter: public QObject, public IAudioFilter
{
    Q_OBJECT
public:
    DigitalAudioFilter();
signals:
    void output(AudioBuffer buf);
public slots:
    AudioBuffer input(const AudioBuffer& buf);
private:
    QVector<real_t> filter_num;
    QVector<real_t> filter_denum;
};

#endif // FILTER_H
