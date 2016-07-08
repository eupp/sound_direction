/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
