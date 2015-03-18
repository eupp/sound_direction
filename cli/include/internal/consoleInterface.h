#pragma once

#include <QObject>

#include "trikSound/iSettingsProvider.h"

class ConsoleInterface: public QObject, public ISettingsProvider
{
    Q_OBJECT
public:



signals:

    void updateAngleDetectionHistoryDepth(int historyDepth);
    void updateWindowSize(size_t windowSize);
    void updateVolume(double vol);

private:


};
