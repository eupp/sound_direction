#ifndef ANGLEDETECTORIMPL_H
#define ANGLEDETECTORIMPL_H

#include <array>
#include <memory>
#include <cassert>

#include <boost/circular_buffer.hpp>

class AngleDetectorImpl
{
public:

    static const size_t offset = 40;
    static const size_t corrSize = 2 * offset + 1;
    static const size_t historySize = 5;

    typedef long long corr_type;
    typedef std::array<corr_type, corrSize> corr_array;

    AngleDetectorImpl():
        mCorrHistory(historySize)
      , mHypthHistory(historySize)
    {};


    template <typename T>
    void handleWindow(T first1, T last1, T first2, T last2)
    {
        const corr_array corr = calcCorrelation(first1, last1, first2, last2);
        mCorrHistory.push_back(corr);
        corr_type hypth = calcPeakPos();
        mHypthHistory.push_back(hypth);
    }

private:
    template <typename T>
    corr_array calcCorrelation(T first1, T last1, T first2, T last2)
    {
        const std::ptrdiff_t windowSize = last1 - first1;
        const std::ptrdiff_t windowSize2 = last2 - first2;
        assert(windowSize == windowSize2);

        corr_array corr;
        corr.fill(0);

        T u = first1;
        T v = first2;
        for (size_t i = 0; i < corrSize; i++) {
            for (size_t j = 0; j < windowSize; ++j) {
                corr[i] += u[i + j] * v[j];
            }
        }
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

        auto maxCorr = std::max_element(corrRes.begin(), corrRes.end());
        std::ptrdiff_t diff = maxCorr - corrRes.begin();
        // invert position of peak and subset offset from center
        return (corrSize - 1 - diff) - offset;
    }

    boost::circular_buffer<corr_array> mCorrHistory;
    boost::circular_buffer<corr_type> mHypthHistory;
};

#endif // ANGLEDETECTORIMPL_H
