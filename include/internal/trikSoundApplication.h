#ifndef TRIKSOUNDAPPLICATION_H
#define TRIKSOUNDAPPLICATION_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QTextStream>

class TrikSoundApplication: public QObject
{
    Q_OBJECT
public:
    enum Command {
        NO_COMMAND,
        LISTEN_FILE,
        LIST_DEVICES
    };

    TrikSoundApplication(QObject* parent);
public slots:
    void run();
signals:
    void finished();
private:
    void parseArgs();
    bool listenWavFile();
    void printAllDevicesInfo();
    void printDeviceInfo(const QAudioDeviceInfo& info);

    Command mCmd;
    QString mFilename;
    double mMicrDist;

    QTextStream out;
};

#endif // TRIKSOUNDAPPLICATION_H
