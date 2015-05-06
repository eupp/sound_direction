#pragma once

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT ISettingsProvider
{
public:

    virtual ~ISettingsProvider() {}

public slots:

    virtual void processEvents() = 0;

signals:

    virtual void updateAngleDetectionHistoryDepth(int historyDepth) = 0;
    virtual void updateWindowSize(quint64 size) = 0;
    virtual void updateVolume(double vol) = 0;

};

}
