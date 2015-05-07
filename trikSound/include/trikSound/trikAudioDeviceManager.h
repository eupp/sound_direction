#pragma once

#include "audioDeviceManager.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikAudioDeviceManager: public AudioDeviceManager
{
public:
    TrikAudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                           const QAudioFormat& audioFormat,
                           const std::shared_ptr<QIODevice>& buffer,
                           bool forceInit = false);

    double volume() const;
    void setVolume(double vol);

private:
    static const int mMaxVolume;

    double mVolume;
};

}
