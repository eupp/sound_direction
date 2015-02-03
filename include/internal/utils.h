#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

#include <QtGlobal>
#include <QAudioFormat>
#include <QDebug>

template <typename T>
void debug_print(const char* filename, T* array, size_t size)
{
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }
    for (size_t i = 0; i < size; i++) {
        file << array[i] << std::endl;
    }
}

inline qint64 bytesForDuration(qint64 duration, const QAudioFormat& format)
{
//    qDebug() << format.sampleRate() << " " << format.sampleSize()  << " " << format.channelCount();
    return (duration * format.sampleRate() * (format.sampleSize() / 8) * format.channelCount()) / 1000;
}

#endif // UTILS_H
