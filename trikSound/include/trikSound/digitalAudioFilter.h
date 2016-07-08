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
#include <iterator>

#include "triksound_global.h"
#include "audioFilter.h"
#include "digitalAudioFilterImpl.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT DigitalAudioFilter : public AudioFilter<Iter>
{
public:

    typedef typename AudioFilter<Iter>::FilterPtr FilterPtr;

    DigitalAudioFilter();

protected:

    void handleWindowImpl(Iter first, Iter last);

private:
    std::unique_ptr<DigitalAudioFilterImpl<Iter>> mImpl;
};

template <typename Iter>
DigitalAudioFilter<Iter>::DigitalAudioFilter():
    mImpl(new DigitalAudioFilterImpl<Iter>())
{}

template <typename Iter>
void DigitalAudioFilter<Iter>::handleWindowImpl(Iter first, Iter last)
{
    mImpl->handleWindowImpl(first, last, typename std::iterator_traits<Iter>::iterator_category());
}

}
