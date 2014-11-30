#ifndef TRIKSOUNDAPPLICATION_H
#define TRIKSOUNDAPPLICATION_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QTextStream>
#include <QSharedPointer>

#include "include/internal/audioDeviceManager.h"

class TrikSoundApplication: public QObject
{
    Q_OBJECT
public:
    enum Command {
        NO_COMMAND,
        LISTEN_FILE,
        LISTEN_MICR,
        RECORD_FILE,
        LIST_DEVICES
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
    void initAudioDevice();
    void printAllDevicesInfo();
    void printDeviceInfo(const QAudioDeviceInfo& info);

    // arguments
    Command mCmd;
    QString mFilename;
    double mMicrDist;
    size_t mDuration;

    QSharedPointer<AudioDeviceManager> mDeviceManager;

    QTextStream out;
};

#endif // TRIKSOUNDAPPLICATION_H
