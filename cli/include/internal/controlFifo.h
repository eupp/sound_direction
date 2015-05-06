#pragma once

#include <cstdio>
#include <iostream>
#include <memory>

#include <QObject>
#include <QSocketNotifier>

#include "trikSound/iSettingsProvider.h"

class ControlFifo: public QObject, public trikSound::ISettingsProvider
{
    Q_OBJECT
public:

    ControlFifo();

public slots:

    void processEvents();

signals:

    void updateAngleDetectionHistoryDepth(int historyDepth);
    void updateWindowSize(quint64 size);
    void updateVolume(double vol);

private slots:

    void readData();

private:

    static const size_t CMD_BUFFER_SIZE = 400;

    void readCommand();

    FILE* mFile;
    QSocketNotifier mNotifier;
    char mCmdBuffer[CMD_BUFFER_SIZE];
};
