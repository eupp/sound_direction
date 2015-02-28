#ifndef TRIKSOUNDAPPLICATION_H
#define TRIKSOUNDAPPLICATION_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QTextStream>
#include <QSharedPointer>

#include "trikSound/audioDeviceManager.h"
#include "trikSound/iAudioFilter.h"
#include "trikSound/audioPipe.h"

class TrikSoundApplication: public QObject
{
    Q_OBJECT
public:
    enum Command {
        NO_COMMAND,
        LISTEN_FILE,
        LISTEN_MICR,
        RECORD_FILE,
        LIST_DEVICES,
        BENCHMARK
    };

    TrikSoundApplication(QObject* parent);
public slots:
    void run();
signals:
    void finished();
private slots:
    void stopRecording();
private:
    void parseArgs();
    bool listenWavFile();
    void listen();
    void record();
    void benchmark();
    void initAudioDevice();
    void printAllDevicesInfo();
    void printDeviceInfo(const QAudioDeviceInfo& info);

    // arguments
    Command mCmd;
    QString mFilename;
    double mMicrDist;
    double mThreshold;
    size_t mDuration;
    size_t mFrameLength;
    bool mSplitChFlag;

    // flags
    bool mDebugPrintFlag;

    QSharedPointer<trikSound::AudioDeviceManager> mDeviceManager;
    QSharedPointer<trikSound::IAudioFilter> mCapture;
    QSharedPointer<trikSound::IAudioFilter> mDetector;
//    AudioPipe mPipe;

    QTextStream mOut;
};

#endif // TRIKSOUNDAPPLICATION_H
