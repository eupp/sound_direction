#ifndef SOUNDANGLESENSOR_H
#define SOUNDANGLESENSOR_H

#include <QObject>

class SoundAngleSensor: public QObject
{
    Q_OBJECT
public:
    SoundAngleSensor(QObject* parent):
        QObject(parent)
    {}
public slots:
    void run();
signals:
    void finished();
private:
    void parseArgs();

    QString mFilename;
    double mMicrDist;
};

#endif // SOUNDANGLESENSOR_H
