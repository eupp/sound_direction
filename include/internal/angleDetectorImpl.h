#ifndef ANGLEDETECTORIMPL_H
#define ANGLEDETECTORIMPL_H

#include <array>
#include <memory>
#include <cassert>

#include <boost/circular_buffer.hpp>

#include "include/internal/debugUtils.h"

template <typename C>
class AngleDetectorImpl
{
public:

    typedef typename C::iterator iterator_type;

    static const int offset = 40;
    static const int corrSize = 2 * offset + 1;
    static const size_t historySize = 5;

    typedef long long corr_type;
    typedef std::array<corr_type, corrSize> corr_array;

    AngleDetectorImpl():
        mCorrHistory(historySize)
      , mHypthHistory(historySize)
    {};

    void handleWindow(iterator_type first1, iterator_type last1,
                      iterator_type first2, iterator_type last2)
    {
        const corr_array corr = calcCorrelation(first1, last1, first2, last2);
        mCorrHistory.push_back(corr);
        corr_type hypth = calcPeakPos();
        mHypthHistory.push_back(hypth);
    }

    int getHypothesis(size_t historyDepth);

private:

    corr_array calcCorrelation(iterator_type first1, iterator_type last1,
                               iterator_type first2, iterator_type last2)
    {
        const std::ptrdiff_t windowSize = last1 - first1;
        const std::ptrdiff_t windowSize2 = last2 - first2;
        assert(windowSize == windowSize2);

        corr_array corr;
        corr.fill(0);

        iterator_type u = first1;
        iterator_type v = first2 + offset;
        for (size_t i = 0; i < corrSize; i++) {
            for (size_t j = 0; j < windowSize; ++j) {
                corr[i] += u[i + j] * v[j];
            }
        }

        dprint_sequence("corr_orig.test", corr.begin(), corr.end());

        return corr;
    }

    int calcPeakPos()
    {
        corr_array corrRes;
        corrRes.fill(0);

        for (auto& corr : mCorrHistory) {
            for (size_t i = 0; i < corrSize; ++i) {
                corrRes[i] += corr[i];
            }
        }

        dprint_sequence("corr.test", corrRes.begin(), corrRes.end());

        auto maxCorr = std::max_element(corrRes.begin(), corrRes.end());
        std::ptrdiff_t diff = maxCorr - corrRes.begin();
        // invert position of peak and subset offset from center
        return (corrSize - 1 - diff) - offset;
    }

    boost::circular_buffer<corr_array> mCorrHistory;
    boost::circular_buffer<int> mHypthHistory;
};

template <typename C>
int AngleDetectorImpl<C>::getHypothesis(size_t historyDepth)
{
    return mHypthHistory.at(mHypthHistory.size() - 1 - historyDepth);
}

#endif // ANGLEDETECTORIMPL_H


