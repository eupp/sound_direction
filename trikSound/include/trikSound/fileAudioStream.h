#pragma once

#include <vector>

#include "triksound_global.h"
#include "audioStream.h"
#include "wavFile.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT FileAudioStream: public AudioStream
{
public:

    FileAudioStream(const QString& filename, size_t windowSize, QObject* parent = 0);

    void run();
    void stop();
    void read(sample_type* buf);
    // count of samples available for reading per channels (i.e. channel[i].samplesAvailable() )
    size_t samplesAvailable() const;

    size_t windowSize() const;
    void setWindowSize(size_t size);

private:

    WavFile mFile;
    std::vector<sample_type> mWindow;
};

}
