#pragma once

#include <array>
#include <memory>
#include <cassert>

#include <boost/circular_buffer.hpp>

#include "../trikSound/debugUtils.h"

namespace trikSound {

template <typename C>
class AngleDetectorImpl
{
public:

    typedef typename C::iterator iterator_type;

    static const int offset = 40;
    static const int corrSize = 2 * offset + 1;
    static const size_t historySize = 400;

    typedef long long corr_type;
    typedef std::array<corr_type, corrSize> corr_array;

    AngleDetectorImpl(const QAudioFormat& format, double micrDist);

    void handleWindow(iterator_type first1, iterator_type last1,
                      iterator_type first2, iterator_type last2);

    double getAngle(size_t historyDepth);

    int getHypothesis(size_t historyDepth);

private:

    corr_array calcCorrelation(iterator_type first1, iterator_type last1,
                               iterator_type first2, iterator_type last2);
    int calcPeakPos(size_t historyDepth);

    const int mSampleRate;
    const int mMicrDist;

    boost::circular_buffer<corr_array> mCorrHistory;
    boost::circular_buffer<int> mHypthHistory;
};

template <typename C>
AngleDetectorImpl<C>::AngleDetectorImpl(const QAudioFormat& format, double micrDist):
    mSampleRate(format.sampleRate())
  , mMicrDist(micrDist)
  , mCorrHistory(historySize)
  , mHypthHistory(historySize)
{}

template <typename C>
void AngleDetectorImpl<C>::handleWindow(iterator_type first1,
                                        iterator_type last1,
                                        iterator_type first2,
                                        iterator_type last2)
{
    const corr_array corr = calcCorrelation(first1, last1, first2, last2);
    mCorrHistory.push_back(corr);
}

template <typename C>
double AngleDetectorImpl<C>::getAngle(size_t historyDepth)
{
    int fs = mSampleRate;

    int peak = calcPeakPos(historyDepth);

    double a = (double(peak) * 33000) / (2 * fs);
    double c = mMicrDist;
    double pi = 3.14159265359;

    double phi = 0;
    if (std::abs(a) > std::abs(c)) {
        // sign(a) * pi/2
        phi = ((a > 0) - (a < 0)) * pi/2;
    }
    else {
        phi = pi/2 - acos(a/c);
    }

    return (phi * 180) / pi;
}

template <typename C>
int AngleDetectorImpl<C>::getHypothesis(size_t historyDepth)
{
    return mHypthHistory.at(mHypthHistory.size() - 1 - historyDepth);
}

template <typename C>
typename AngleDetectorImpl<C>::corr_array AngleDetectorImpl<C>::calcCorrelation(iterator_type first1,
                                                                                iterator_type last1,
                                                                                iterator_type first2,
                                                                                iterator_type last2)
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

    return corr;
}

template <typename C>
int AngleDetectorImpl<C>::calcPeakPos(size_t historyDepth)
{
    corr_array corrRes;
    corrRes.fill(0);

    assert(historyDepth <= mCorrHistory.size());

    auto end = mCorrHistory.rbegin();
    advance(end, historyDepth);
    for (auto itr = mCorrHistory.rbegin(); itr != end; ++itr) {
        for (size_t i = 0; i < corrSize; ++i) {
            corrRes[i] += (*itr)[i];
        }
    }

    dprint_sequence("corr.test", corrRes.begin(), corrRes.end());

    auto maxCorr = std::max_element(corrRes.begin(), corrRes.end());
    std::ptrdiff_t diff = maxCorr - corrRes.begin();
    // invert position of peak and subset offset from center
    return (corrSize - 1 - diff) - offset;
}

}


