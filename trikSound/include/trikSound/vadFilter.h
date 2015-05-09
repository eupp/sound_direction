#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioFilter.h"
#include "vadFilterImpl.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT VadFilter: public AudioFilter<Iter>
{
public:

    VadFilter():
        mImpl(new VadFilterImpl<Iter>())
    {}

    double getEnergyCoefficient() const
    {
        return mImpl->getEnergyCoefficient();
    }

protected:

    void handleWindowImpl(Iter first, Iter last);

private:
    std::unique_ptr<VadFilterImpl<Iter>> mImpl;
};

template <typename Iter>
void VadFilter<Iter>::handleWindowImpl(Iter first, Iter last)
{
    mImpl->handleWindowImpl(first, last);
}

}

