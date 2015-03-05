#pragma once

#include <iostream>
#include <fstream>

#include <QtGlobal>
#include <QAudioFormat>

namespace trikSound {

inline qint64 samplesForDuration(qint64 duration, const QAudioFormat& format)
{
    return (duration * format.sampleRate() * format.channelCount()) / 1000;
}


inline qint64 bytesForDuration(qint64 duration, const QAudioFormat& format)
{
    return (duration * format.sampleRate() * (format.sampleSize() / 8) * format.channelCount()) / 1000;
}

}
