#pragma once

#include <iterator>
#include <limits>
#include <utility>

#include <boost/circular_buffer.hpp>

namespace trikSound
{

template <typename Iter>
class VadFilterImpl
{
private:

    typedef long long energy_type;

public:

    typedef typename std::iterator_traits<Iter>::value_type value_type;

    explicit VadFilterImpl(double threshold):
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

    double calcEnrgCoef() const
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
        energy_type frameMax = SQR_MAX * frameSize;
        return (double) enrg / frameMax;
    }

    boost::circular_buffer<frame_params> mEnrgBuf;
    double mThreshold;
};

}
