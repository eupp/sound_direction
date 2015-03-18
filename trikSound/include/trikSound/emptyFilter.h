#pragma once

#include "audioFilter.h"

namespace trikSound
{

template <typename Iter>
class EmptyFilter: public AudioFilter<Iter>
{
public:

    typedef std::shared_ptr<AudioFilter<Iter>> FilterPtr;

    EmptyFilter(const FilterPtr& prevFilter = FilterPtr()):
        AudioFilter<Iter>(prevFilter)
    {}

protected:

    void handleWindowImpl(Iter first, Iter last) override
    {
        Q_UNUSED(first);
        Q_UNUSED(last);
        return;
    }
};

}
