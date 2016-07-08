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
#include <utility>

#include "triksound_global.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AudioFilter
{
public:

    typedef std::shared_ptr<AudioFilter<Iter>> FilterPtr;

    AudioFilter();
    virtual ~AudioFilter() {}

    void handleWindow(Iter first, Iter last);

    virtual bool framePromotion() const
    {
        return true;
    }

    virtual void reset() {}

    void setUpdated(bool updated);
    bool updated() const;

protected:

    virtual void handleWindowImpl(Iter first, Iter last) = 0;

private:
    FilterPtr mPrev;
    bool mUpdated;
};

template <typename Iter>
AudioFilter<Iter>::AudioFilter():
    mUpdated(false)
{}

template <typename Iter>
void AudioFilter<Iter>::handleWindow(Iter first, Iter last)
{
    handleWindowImpl(first, last);
}

template <typename Iter>
void AudioFilter<Iter>::setUpdated(bool updated)
{
    mUpdated = updated;
}

template <typename Iter>
bool AudioFilter<Iter>::updated() const
{
    return mUpdated;
}

}
