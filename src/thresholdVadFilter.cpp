#include "include/internal/thresholdVadFilter.h"

#include <algorithm>

#include <QDebug>

ThresholdVadFilter::ThresholdVadFilter(double threshold):
    mThreshold(threshold)
{}

double ThresholdVadFilter::threshold() const
{
    return mThreshold;
}

void ThresholdVadFilter::setThreshold(double threshold)
{
    mThreshold = threshold;
}


template <typename T>
bool checkFrameThreshold(T threshold, const T* frame, size_t length)
{
    const T* pMax = std::max_element(frame, frame + length);
    const T* pMin = std::min_element(frame, frame + length);
    T mMax = std::abs(*pMax);
    T mMin = std::abs(*pMin);
    if ((mMax > threshold) || (mMin > threshold)) {
        return true;
    }
    return false;
}

bool ThresholdVadFilter::vad(const AudioBuffer& buf)
{
    bool isActive = false;
    if (buf.audioFormat().sampleSize() == 8) {
        // hardcode -- unsigned char
        const quint8* frame = reinterpret_cast<const quint8*>(buf.data());
        quint8 thrld = mThreshold * std::numeric_limits<quint8>::max();
        isActive = checkFrameThreshold(thrld, frame, buf.sampleCount());
    }
    else if (buf.audioFormat().sampleSize() == 16) {
        // hardcode -- signed short
        const qint16* frame = reinterpret_cast<const qint16*>(buf.data());
        qint16 thrld = mThreshold * std::numeric_limits<qint16>::max();
        isActive = checkFrameThreshold(thrld, frame, buf.sampleCount());
    }

    qDebug() << "Threshold Vad Filter: " << isActive;

    return isActive;
}

AudioBuffer ThresholdVadFilter::input(const AudioBuffer& buf)
{
    bool isActive = vad(buf);

    if (isActive) {
        emit output(buf);
        return buf;
    }
    else {
        return AudioBuffer(QByteArray(), QAudioFormat());
    }
}
