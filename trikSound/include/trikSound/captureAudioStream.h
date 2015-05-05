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
                       size_t windowSize,
                       QObject* parent = 0);

    void run();
    void restart();
    void stop();
    void read(sample_type *buf);
    size_t samplesAvailable() const;

    size_t windowSize() const;
    void setWindowSize(size_t size);

private:

    AudioDeviceManagerPtr mDevice;
    CircularBufferQAdapterPtr mBuffer;
    size_t mWindowSize;
};

}
