#pragma once

#include "stereoAudioFilter.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT EmptyStereoFilter: public StereoAudioFilter<Iter>
{
public:

    typedef std::pair<Iter, Iter> range_type;
    typedef std::shared_ptr<StereoAudioFilter<Iter>> FilterPtr;

    EmptyStereoFilter(const FilterPtr& prevFilter = FilterPtr()):
        StereoAudioFilter<Iter>(prevFilter)
    {}

protected:

    void handleWindowImpl(range_type channel1, range_type channel2) override
    {
        Q_UNUSED(channel1);
        Q_UNUSED(channel2);
        return;
    }
};

}
