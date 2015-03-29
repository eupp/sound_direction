#pragma once

#include <memory>
#include <array>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "recordFilter.h"
#include "utils.h"


namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT StereoRecordFilter: public StereoAudioFilter<Iter>

{
public:

    using typename StereoAudioFilter<Iter>::FilterPtr;
    using typename StereoAudioFilter<Iter>::range_type;

    typedef typename RecordFilter<Iter>::WavFilePtr WavFilePtr;

    class InitException: public TrikSoundException
    {
    public:
        InitException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    StereoRecordFilter(const WavFilePtr& wavFile, const FilterPtr &prevFilter = FilterPtr());

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:
    RecordFilter<Iter> mRecordFilter;
};

template <typename Iter>
StereoRecordFilter<Iter>::StereoRecordFilter(const WavFilePtr& wavFile,
                                             const typename StereoAudioFilter<Iter>::FilterPtr& prevFilter):
    StereoAudioFilter<Iter>(prevFilter)
  , mRecordFilter(wavFile)
{}

template <typename Iter>
void StereoRecordFilter<Iter>::handleWindowImpl(range_type channel1,
                                                range_type channel2)
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;

    std::array<range_type, 2> channels;
    int size = std::distance(channel1.first, channel1.second) + std::distance(channel2.first, channel2.second);
    std::vector<value_type> buf(size);

    channels[0] = channel1;
    channels[1] = channel2;
    interweaveChannels<2>(channels, buf.begin());

    mRecordFilter.write(buf);
}

}
