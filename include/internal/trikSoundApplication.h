#ifndef TRIKSOUNDAPPLICATION_H
#define TRIKSOUNDAPPLICATION_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QTextStream>
#include <QSharedPointer>


#include "include/internal/audioDeviceManager.h"
#include "include/internal/iAudioFilter.h"
#include "include/internal/audioPipe.h"

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

    QSharedPointer<AudioDeviceManager> mDeviceManager;
    QSharedPointer<IAudioFilter> mCapture;
    QSharedPointer<IAudioFilter> mDetector;
//    AudioPipe mPipe;

    QTextStream mOut;
};

#endif // TRIKSOUNDAPPLICATION_H
