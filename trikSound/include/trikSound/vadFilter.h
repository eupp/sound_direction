#pragma once

#include <memory>

#include "triksound_global.h"
#include "audioFilter.h"
#include "vadFilterImpl.h"
#include "types.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT VadFilter: public AudioFilter<Iter>
{
public:

    explicit VadFilter(threshold_type threshold):
        mImpl(new VadFilterImpl<Iter>(threshold))
    {}

    threshold_type getEnergyCoefficient() const
    {
        return mImpl->getEnergyCoefficient();
    }

    bool isActive() const
    {
        return mImpl->isActive();
    }

    bool framePromotion() const
    {
        return isActive();
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
