#pragma once

#include "triksound_global.h"
#include "audioFilter.h"
#include "utils.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT SplitFilter : public AudioFilter<Iter>
{
public:

    SplitFilter(const AudioFilter::ptrFilter& prevFilter = ptrFilter());

protected:

    AudioFilter::range_type handleWindowImpl(Iter first, Iter last);

};

}
