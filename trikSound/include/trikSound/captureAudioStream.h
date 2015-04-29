#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioStream.h"
#include "audioDeviceManager.h"
#include "circularBufferQAdapter.h"


namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT CaptureAudioStream: public AudioStream
{
public:

    typedef std::shared_ptr<AudioDeviceManager>  AudioDeviceManagerPtr;
    typedef std::shared_ptr<CircularBufferQAdapter>  CircularBufferQAdapterPtr;

    CaptureAudioStream(AudioDeviceManagerPtr device,
                       CircularBufferQAdapterPtr buffer,
                       QObject* parent = 0);

    void run();
    void restart();
    void stop();
    void read(sample_type *buf, size_t size);
    size_t samplesAvailable() const;

private:

    AudioDeviceManagerPtr mDevice;
    CircularBufferQAdapterPtr mBuffer;

};

}
