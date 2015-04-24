#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioFilter.h"
#include "stereoAudioFilter.h"
#include "audioPipe.h"
#include "utils.h"


namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT SplitFilter : public StereoAudioFilter<Iter>
{
public:

    using typename StereoAudioFilter<Iter>::range_type;
    using typename StereoAudioFilter<Iter>::FilterPtr;

    typedef std::shared_ptr<AudioPipe<Iter>> AudioPipePtr;

    SplitFilter(const AudioPipePtr& pipe = AudioPipePtr());

protected:

    void handleWindowImpl(range_type channel1,
                          range_type channel2);

private:
    AudioPipePtr mPipe;
};

template <typename Iter>
SplitFilter<Iter>::SplitFilter(const AudioPipePtr& pipe):
    mPipe(pipe)
{}

template <typename Iter>
void SplitFilter<Iter>::handleWindowImpl(range_type channel1,
                                         range_type channel2)
{
    if (mPipe) {
        mPipe->handleWindow(channel1.first, channel1.second);
        mPipe->handleWindow(channel2.first, channel2.second);
    }
}

}
