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

#include <QAudioFormat>

#include "triksound_global.h"
#include "stereoAudioFilter.h"
#include "angleDetectorImpl.h"
#include "types.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AngleDetector : public StereoAudioFilter<Iter>
{
public:

    typedef typename StereoAudioFilter<Iter>::range_type range_type;
    typedef typename StereoAudioFilter<Iter>::FilterPtr FilterPtr;

    class IncorrectSignalException : public TrikSoundException
    {
    public:
        IncorrectSignalException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    AngleDetector();

    AngleDetector(int sampleRate, double micrDist, int historyDepth = 1);

    void setHistoryDepth(int historyDepth);
    int historyDepth() const;

    void setSampleRate(int sampleRate);
    int sampleRate() const;

    void setMicrDist(double micrDistance);
    double micrDistance() const;

    double getAngle();

    void reset();

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:
    std::unique_ptr<AngleDetectorImpl<Iter>> mImpl;
};

template <typename Iter>
AngleDetector<Iter>::AngleDetector():
    AngleDetector(-1, -1, 1)
{}

template <typename Iter>
AngleDetector<Iter>::AngleDetector(int sampleRate, double micrDist, int historyDepth):
    mImpl(new AngleDetectorImpl<Iter>(sampleRate, micrDist, historyDepth))
{}

template <typename Iter>
void AngleDetector<Iter>::setHistoryDepth(int historyDepth)
{
    mImpl->setHistoryDepth(historyDepth);
}

template <typename Iter>
int AngleDetector<Iter>::historyDepth() const
{
    return mImpl->historyDepth();
}

template <typename Iter>
void AngleDetector<Iter>::setSampleRate(int sampleRate)
{
    mImpl->setSampleRate(sampleRate);
}

template <typename Iter>
int AngleDetector<Iter>::sampleRate() const
{
    return mImpl->sampleRate();
}

template <typename Iter>
void AngleDetector<Iter>::setMicrDist(double micrDistance)
{
    mImpl->setMicrDist(micrDistance);
}

template <typename Iter>
double AngleDetector<Iter>::micrDistance() const
{
    return mImpl->micrDistance();
}

template <typename Iter>
double AngleDetector<Iter>::getAngle()
{
    return mImpl->getAngle();
}

template <typename Iter>
void AngleDetector<Iter>::reset()
{
    mImpl->reset();
}

template <typename Iter>
void AngleDetector<Iter>::handleWindowImpl(range_type channel1, range_type channel2)
{
    try {
        mImpl->handleWindowImpl(channel1, channel2, typename std::iterator_traits<Iter>::iterator_category());
    }
    catch (typename AngleDetectorImpl<Iter>::IncorrectSignalException& exc) {
        throw IncorrectSignalException(exc.what());
    }
}

}
