#ifndef TRIKSOUNDAPPLICATION_H
#define TRIKSOUNDAPPLICATION_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QTextStream>

class TrikSoundApplication: public QObject
{
    Q_OBJECT
public:
    TrikSoundApplication(QObject* parent);
public slots:
    void run();
signals:
    void finished();
private:
    void parseArgs();
    void printAllDevicesInfo();
    void printDeviceInfo(const QAudioDeviceInfo& info);

    QString mFilename;
    double mMicrDist;
    QTextStream out;
};

#endif // TRIKSOUNDAPPLICATION_H
