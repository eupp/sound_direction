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

#include <iterator>
#include <limits>
#include <utility>

#include <boost/circular_buffer.hpp>

#include "../trikSound/types.h"

namespace trikSound
{

template <typename Iter>
class VadFilterImpl
{
private:

    typedef long long energy_type;

public:

    typedef typename std::iterator_traits<Iter>::value_type value_type;

    explicit VadFilterImpl(threshold_type threshold):
        mEnrgBuf(HISTORY_DEPTH)
      , mThreshold(threshold)
    {}

    void handleWindowImpl(Iter first, Iter last)
    {
        auto windowSize = std::distance(first, last);
        if (windowSize == 0) {
            return;
        }

        energy_type enrg = 0;
        for (; first != last; ++first) {
            energy_type x = static_cast<energy_type>(*first);
            enrg += x * x;
        }

        mEnrgBuf.push_back(std::make_pair(enrg, windowSize));
    }

    double getEnergyCoefficient() const
    {
        return calcEnrgCoef();
    }

    bool isActive() const
    {
        return mThreshold < calcEnrgCoef();
    }

private:

    typedef std::pair<energy_type, int> frame_params;

    static const energy_type SQR_MAX = static_cast<energy_type>(std::numeric_limits<value_type>::max())
                                     * static_cast<energy_type>(std::numeric_limits<value_type>::max());

    static const int HISTORY_DEPTH = 5;

    threshold_type calcEnrgCoef() const
    {
        if (mEnrgBuf.empty()) {
            return 0;
        }
        energy_type enrg = 0;
        int frameSize = 0;
        for (auto it = mEnrgBuf.begin(); it != mEnrgBuf.end(); ++it) {
            enrg += it->first;
            frameSize += it->second;
        }
        return enrg / frameSize;
    }

    boost::circular_buffer<frame_params> mEnrgBuf;
    threshold_type mThreshold;
};

}
