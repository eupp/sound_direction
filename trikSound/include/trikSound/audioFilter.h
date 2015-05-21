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
