#pragma once

#include <QObject>

#include "triksound_global.h"

class TRIKSOUNDSHARED_EXPORT SettingsProvider: public QObject
{
    Q_OBJECT

    SettingsProvider(QObject* parent = 0):
        QObject(parent)
    {}
    virtual ~SettingsProvider() {}

signals:

    void updateAngleDetectionHistoryDepth(int historyDepth);
    void updateWindowSize(size_t size);
    void updateVolume(double vol);

};
