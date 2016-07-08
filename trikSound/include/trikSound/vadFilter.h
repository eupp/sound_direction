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
