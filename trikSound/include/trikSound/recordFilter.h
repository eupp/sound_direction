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

#include <iterator>
#include <vector>
#include <memory>

#include <QString>

#include "triksound_global.h"
#include "trikSoundException.h"
#include "audioFilter.h"
#include "wavFile.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT RecordFilter: public AudioFilter<Iter>
{
public:

    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef typename AudioFilter<Iter>::FilterPtr FilterPtr;
    typedef std::shared_ptr<WavFile> WavFilePtr;

    class InitException: public TrikSoundException
    {
    public:
        InitException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    RecordFilter(const WavFilePtr& wavFile);

    void write(const std::vector<value_type>& buf);

private:

    void handleWindowImpl(Iter first, Iter last);

private:
    WavFilePtr mWavFile;
};

template <typename Iter>
RecordFilter<Iter>::RecordFilter(const WavFilePtr& wavFile):
    mWavFile(wavFile)
{
    if (!mWavFile->isWritable()) {
        throw InitException("StereoRecordFilter error. Wav file is not writable");
    }
    if (mWavFile->audioFormat().sampleSize() != 8 * sizeof(value_type)) {
        throw InitException("RecordFilter error. Incorrect format for specified type of range");
    }
}

template <typename Iter>
void RecordFilter<Iter>::write(const std::vector<typename RecordFilter<Iter>::value_type>& buf)
{
    mWavFile->write((char*) buf.data(), buf.size() * sizeof(value_type));
}

template <typename Iter>
void RecordFilter<Iter>::handleWindowImpl(Iter first, Iter last)
{
    std::vector<value_type> buf(first, last);
    write(buf);
}

}
