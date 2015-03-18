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

    AngleDetector(const FilterPtr& prevFilter = FilterPtr());

    AngleDetector(int sampleRate, double micrDist, int historyDepth = 1,
                  const FilterPtr& prevFilter = FilterPtr());

    void setHistoryDepth(int historyDepth);
    int historyDepth() const;

    void setSampleRate(int sampleRate);
    int sampleRate() const;

    void setMicrDist(double micrDistance);
    double micrDistance() const;

    double getAngle();

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:
    std::unique_ptr<AngleDetectorImpl<Iter>> mImpl;
};

template <typename Iter>
AngleDetector<Iter>::AngleDetector(const FilterPtr& prevFilter):
    AngleDetector(-1, -1, 1, prevFilter)
{}

template <typename Iter>
AngleDetector<Iter>::AngleDetector(int sampleRate, double micrDist, int historyDepth,
                                   const FilterPtr& prevFilter):
    StereoAudioFilter<Iter>(prevFilter)
  , mImpl(new AngleDetectorImpl<Iter>(sampleRate, micrDist, historyDepth))
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
