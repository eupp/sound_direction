#ifndef TRIKAUDIODEVICEMANAGER_H
#define TRIKAUDIODEVICEMANAGER_H

#include "audioDeviceManager.h"

class TrikAudioDeviceManager: public AudioDeviceManager
{
public:
    TrikAudioDeviceManager(const QAudioDeviceInfo& deviceInfo, const QAudioFormat& audioFormat, size_t bufCapacity);

    double volume() const;
    void setVolume(double vol);

private:
    static const int mMaxVolume;

    double mVolume;
};

#endif // TRIKAUDIODEVICEMANAGER_H
