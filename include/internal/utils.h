#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

#include <QtGlobal>
#include <QAudioFormat>

inline qint64 bytesForDuration(qint64 duration, const QAudioFormat& format)
{
    return (duration * format.sampleRate() * (format.sampleSize() / 8) * format.channelCount()) / 1000;
}

#endif // UTILS_H
