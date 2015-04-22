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
