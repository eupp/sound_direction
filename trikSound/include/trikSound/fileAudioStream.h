#pragma once

#include "triksound_global.h"
#include "audioStream.h"
#include "wavFile.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT FileAudioStream: public AudioStream
{
public:

    FileAudioStream(const QString& filename, QObject* parent = 0);

    void run();
    void stop();
    void read(sample_type* buf, size_t size);
    // count of samples available for reading per channels (i.e. channel[i].samplesAvailable() )
    size_t samplesAvailable() const;

private:

    WavFile mFile;

};

}
