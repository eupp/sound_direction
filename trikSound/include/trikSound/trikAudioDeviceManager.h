#pragma once

#include "audioDeviceManager.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikAudioDeviceManager: public AudioDeviceManager
{
public:
    TrikAudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                           const QAudioFormat& audioFormat,
                           size_t bufCapacity);

    double volume() const;
    void setVolume(double vol);

private:
    static const int mMaxVolume;

    double mVolume;
};

}
