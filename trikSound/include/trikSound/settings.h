#pragma once

#include <cstdlib>

#include <QString>
#include <QAudioFormat>

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT Settings
{
public:

    // construct Setting object with default controller settings

    Settings();

    Settings(const Settings& other) = default;

    int angleDetectionHistoryDepth() const;
    void setAngleDetectionHistoryDepth(int angleDetectionHistoryDepth);

    size_t windowSize() const;
    void setWindowSize(const size_t& windowSize);

    double volume() const;
    void setVolume(double volume);

    bool singleChannelFlag() const;
    void setSingleChannelFlag(bool singleChannelFlag);

    bool filteringFlag() const;
    void setFilteringFlag(bool filteringFlag);

    bool angleDetectionFlag() const;
    void setAngleDetectionFlag(bool angleDetectionFlag);

    int sampleRate() const;
    void setSampleRate(int sampleRate);

    int sampleSize() const;
    void setSampleSize(int sampleSize);

    QAudioFormat::SampleType sampleType() const;
    void setSampleType(const QAudioFormat::SampleType& sampleType);

    double micrDist() const;
    void setMicrDist(double micrDist);

    bool recordStreamFlag() const;
    void setRecordStreamFlag(bool recordStreamFlag);

    QString outputWavFilename() const;
    void setOutputWavFilename(const QString& outputWavFilename);

    int duration() const;
    void setDuration(int duration);

    bool durationFlag() const;
    void setDurationFlag(bool durationFlag);

    bool fileInputFlag() const;
    void setFileInputFlag(bool fileInputFlag);

    QString inputWavFilename() const;
    void setInputWavFilename(const QString& inputWavFilename);

    bool audioDeviceInitFlag() const;
    void setAudioDeviceInitFlag(bool audioDeviceInitFlag);

    bool vadFlag() const;
    void setVadFlag(bool vadFlag);

    double vadThreshold() const;
    void setVadThreshold(double vadThreshold);

private:

    // flags

    bool mSingleChannelFlag;
    bool mFilteringFlag;
    bool mAngleDetectionFlag;
    bool mVadFlag;
    bool mRecordStreamFlag;
    bool mFileInputFlag;
    bool mAudioDeviceInitFlag;

    // audio format parameters

    int mSampleRate;
    int mSampleSize;
    QAudioFormat::SampleType mSampleType;

    // controller settings

    int mAngleDetectionHistoryDepth;
    size_t mWindowSize;
    double mVolume;

    // angle detector arguments

    double mMicrDist;

    // vad arguments

    double mVadThreshold;

    // duration settings

    int mDuration;
    bool mDurationFlag;

    // other settings

    QString mInputWavFilename;
    QString mOutputWavFilename;
};

}
