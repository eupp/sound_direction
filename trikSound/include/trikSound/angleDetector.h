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

    class IncorrectSignalException : public TrikSoundException
    {
    public:
        IncorrectSignalException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    AngleDetector(const QAudioFormat& format, double micrDist, int historyDepth,
                  const FilterPtr& prevFilter = FilterPtr());

    void setHistoryDepth(int historyDepth);
    int historyDepth() const;

    double getAngle();

protected:

    void handleWindowImpl(range_type channel1, range_type channel2);

private:
    std::unique_ptr<AngleDetectorImpl<Iter>> mImpl;
};

template <typename Iter>
AngleDetector<Iter>::AngleDetector(const QAudioFormat& format, double micrDist, int historyDepth,
                                   const FilterPtr& prevFilter):
    StereoAudioFilter(prevFilter)
  , mImpl(new AngleDetectorImpl<Iter>(format, micrDist, historyDepth))
{}

template <typename Iter>
void AngleDetector::setHistoryDepth(int historyDepth)
{
    mImpl->setHistoryDepth();
}

template <typename Iter>
int AngleDetector<Iter>::historyDepth() const
{
    return mImpl->getHistoryDepth();
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
        mImpl->handleWindowImpl(channel1, channel2, std::iterator_traits<Iter>::iterator_category());
    }
    catch (AngleDetectorImpl::IncorrectSignalException& exc) {
        throw IncorrectSignalException(exc.what());
    }
}

}
