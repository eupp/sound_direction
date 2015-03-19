#pragma once

#include "triksound_global.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT ISettingsProvider
{
public:

    virtual ~ISettingsProvider() {}

signals:

    virtual void updateAngleDetectionHistoryDepth(int historyDepth) = 0;
    virtual void updateWindowSize(size_t size) = 0;
    virtual void updateVolume(double vol) = 0;

};

}
