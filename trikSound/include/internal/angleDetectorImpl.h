#pragma once

#include <array>
#include <memory>
#include <cassert>
#include <iterator>

#include <boost/circular_buffer.hpp>

#include <QAudioFormat>

#include "../trikSound/debugUtils.h"
#include "../trikSound/trikSoundException.h"
#include "../trikSound/stereoAudioFilter.h"

namespace trikSound {

template <typename Iter>
class AngleDetectorImpl
{
public:

    typedef typename StereoAudioFilter<Iter>::range_type range_type;

    class IncorrectSignalException : public TrikSoundException
    {
    public:
        IncorrectSignalException():
            TrikSoundException("Signal should contain left and right channel")
        {}
    };

    static const int offset = 40;
    static const int corrSize = 2 * offset + 1;
    static const size_t historySize = 40;

    typedef long long corr_type;
    typedef std::array<corr_type, corrSize> corr_array;

    AngleDetectorImpl(int sampleRate, double micrDist, int historyDepth);

    void handleWindowImpl(range_type channel1,
                          range_type channel2,
                          std::random_access_iterator_tag);

    double getAngle();

    int getHypothesis(size_t historyDepth);

    int historyDepth() const;
    void setHistoryDepth(int historyDepth);

    double micrDist() const;
    void setMicrDist(double micrDist);

    int sampleRate() const;
    void setSampleRate(int sampleRate);

    void reset();

private:

    corr_array calcCorrelation(Iter first1, Iter last1,
                               Iter first2, Iter last2);
    int calcPeakPos(size_t historyDepth);

    int mSampleRate;
    double mMicrDist;

    int mHistoryDepth;
    boost::circular_buffer<corr_array> mCorrHistory;
    boost::circular_buffer<int> mHypthHistory;
};

template <typename Iter>
AngleDetectorImpl<Iter>::AngleDetectorImpl(int sampleRate, double micrDist, int historyDepth):
    mSampleRate(sampleRate)
  , mMicrDist(micrDist)
  , mHistoryDepth(historyDepth)
  , mCorrHistory(historySize)
  , mHypthHistory(historySize)
{}

template <typename Iter>
void AngleDetectorImpl<Iter>::handleWindowImpl(typename AngleDetectorImpl<Iter>::range_type channel1,
                                               typename AngleDetectorImpl<Iter>::range_type channel2,
                                               std::random_access_iterator_tag)
{
    const corr_array corr = calcCorrelation(channel1.first, channel1.second,
                                            channel2.first, channel2.second);
    mCorrHistory.push_back(corr);
}

template <typename Iter>
double AngleDetectorImpl<Iter>::getAngle()
{
    int fs = mSampleRate;
    int peak = calcPeakPos(mHistoryDepth);

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

template <typename Iter>
int AngleDetectorImpl<Iter>::getHypothesis(size_t historyDepth)
{
    return mHypthHistory.at(mHypthHistory.size() - 1 - historyDepth);
}

template <typename Iter>
int AngleDetectorImpl<Iter>::historyDepth() const
{
    return mHistoryDepth;
}

template <typename Iter>
void AngleDetectorImpl<Iter>::setHistoryDepth(int historyDepth)
{
    mHistoryDepth = historyDepth;
}

template <typename Iter>
typename AngleDetectorImpl<Iter>::corr_array AngleDetectorImpl<Iter>::calcCorrelation(Iter first1,
                                                                                      Iter last1,
                                                                                      Iter first2,
                                                                                      Iter last2)
{
    std::ptrdiff_t windowSize = last1 - first1;
    std::ptrdiff_t windowSize2 = last2 - first2;

    assert(windowSize == windowSize2);
    assert(windowSize > corrSize);

    // reduce window size for shifting one signal relatively to another safely
    windowSize -= corrSize;


    corr_array corr;
    corr.fill(0);

    Iter u = first1;
    Iter v = first2 + offset;
    for (size_t i = 0; i < corrSize; i++) {
        for (size_t j = 0; j < windowSize; ++j) {
            corr[i] += u[i + j] * v[j];
        }
    }

//    auto dst = corr.begin() + offset;
//    for (size_t i = 0; i < offset + 1; ++i) {
//        Iter u = first1 + i;
//        Iter v = first2;
//        size_t wsize = windowSize - i;
//        for (size_t j = 0; j < wsize; ++j) {
//            dst[i] += (*u)   * (*v);
//            ++u;
//            ++v;
//        }
//    }

//    dst = corr.begin();
//    for (size_t i = 0; i < offset; ++i) {
//        Iter u = first1;
//        Iter v = first2 + i + 1;
//        size_t wsize = windowSize - i;
//        for (size_t j = 0; j < wsize; ++j) {
//            dst[offset - 1 - i] += (*u) * (*v);
//            ++u;
//            ++v;
//        }
//    }

    return corr;
}

template <typename Iter>
int AngleDetectorImpl<Iter>::calcPeakPos(size_t historyDepth)
{
    corr_array corrRes;
    corrRes.fill(0);

//    assert(historyDepth <= mCorrHistory.size());
    historyDepth = std::min(historyDepth, mCorrHistory.size());

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

template <typename Iter>
int AngleDetectorImpl<Iter>::sampleRate() const
{
    return mSampleRate;
}

template <typename Iter>
void AngleDetectorImpl<Iter>::setSampleRate(int sampleRate)
{
    mSampleRate = sampleRate;
}

template <typename Iter>
void AngleDetectorImpl<Iter>::reset()
{
    mCorrHistory.clear();
}

template <typename Iter>
double AngleDetectorImpl<Iter>::micrDist() const
{
    return mMicrDist;
}

template <typename Iter>
void AngleDetectorImpl<Iter>::setMicrDist(double micrDist)
{
    mMicrDist = micrDist;
}


}


