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

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT StereoAudioFilter
{
public:

    typedef std::pair<Iter, Iter> range_type;
    typedef std::shared_ptr<StereoAudioFilter<Iter>> FilterPtr;

    static range_type make_empty_range();

    StereoAudioFilter();
    virtual ~StereoAudioFilter() {}

    void handleWindow(range_type channel1, range_type channel2);

    virtual bool framePromotion() const
    {
        return true;
    }

    virtual void reset() {}

    void setUpdated(bool updated);
    bool updated() const;

protected:
    virtual void handleWindowImpl(range_type channel1, range_type channel2) = 0;

private:
    bool mUpdated;
};

template <typename Iter>
StereoAudioFilter<Iter>::StereoAudioFilter():
    mUpdated(false)
{}

template <typename Iter>
void StereoAudioFilter<Iter>::handleWindow(range_type channel1, range_type channel2)
{
    handleWindowImpl(channel1, channel2);
}

template <typename Iter>
void StereoAudioFilter<Iter>::setUpdated(bool updated)
{
    mUpdated = updated;
}

template <typename Iter>
bool StereoAudioFilter<Iter>::updated() const
{
    return mUpdated;
}

template <typename Iter>
typename StereoAudioFilter<Iter>::range_type StereoAudioFilter<Iter>::make_empty_range()
{
    return make_pair(Iter(), Iter());
}

}

