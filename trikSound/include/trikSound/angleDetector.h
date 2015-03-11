#pragma once

#include <memory>

#include <QAudioFormat>

#include "triksound_global.h"
#include "audioFilter.h"
#include "angleDetectorImpl.h"
#include "types.h"

namespace trikSound {

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AngleDetector : public AudioFilter<Iter>
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
                  const AudioFilter::ptrFilter& prevFilter = AudioFilter::ptrFilter());

    void setHistoryDepth(int historyDepth);
    int historyDepth() const;

    double getAngle();

protected:

    void handleWindowImpl(Iter first, Iter last);

private:
    std::unique_ptr<AngleDetectorImpl<Iter>> mImpl;
};

template <typename Iter>
AngleDetector<Iter>::AngleDetector(const QAudioFormat& format, double micrDist, int historyDepth,
                                   const ptrFilter& prevFilter):
    AudioFilter(prevFilter)
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
void AngleDetector<Iter>::handleWindowImpl(Iter first, Iter last)
{
    try {
        mImpl->handleWindowImpl(first, last);
        return make_pair(first, last);
    }
    catch (AngleDetectorImpl::IncorrectSignalException& exc) {
        throw IncorrectSignalException(exc.what());
    }
}

}
