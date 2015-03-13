#pragma once

#include <array>
#include <vector>
#include <iterator>
#include <utility>

#include "../trikSound/types.h"
#include "../trikSound/audioFilter.h"
#include "realTypeTraits.h"

namespace trikSound {

template <typename Iter>
class DigitalAudioFilterImpl
{
public:

    DigitalAudioFilterImpl();

    void handleWindowImpl(Iter first, Iter last, std::random_access_iterator_tag);

private:

    void filtfilt(Iter first, Iter last);

    template <typename InputIter, typename OutputIter>
    void filterSignal(InputIter first, InputIter last, OutputIter dst);

    const static int filter_size = 7;

    typedef std::vector<real_t> fsignal_type;
    typedef std::array<real_t, filter_size> filter_coeffs;

    filter_coeffs filter_num = {{0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281}};
    filter_coeffs filter_denum = {{0, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999}};

    fsignal_type mTmpSignal1;
    fsignal_type mTmpSignal2;
};

template <typename Iter>
DigitalAudioFilterImpl<Iter>::DigitalAudioFilterImpl()
{
    std::reverse(filter_denum.begin(), filter_denum.end());
    std::reverse(filter_num.begin(), filter_num.end());
}

template <typename Iter>
void DigitalAudioFilterImpl<Iter>::handleWindowImpl(Iter first,
                                                    Iter last,
                                                    std::random_access_iterator_tag)
{
    filtfilt(first, last);
}

template <typename Iter>
void DigitalAudioFilterImpl<Iter>::filtfilt(Iter first, Iter last)
{
    const int n = std::distance(first, last);

    mTmpSignal1.resize(n);
    mTmpSignal2.resize(n);

    filterSignal(first, last, mTmpSignal1.begin());
    std::reverse(mTmpSignal1.begin(), mTmpSignal1.end());
    filterSignal(mTmpSignal1.begin(), mTmpSignal1.end(), mTmpSignal2.begin());

    std::transform(mTmpSignal2.rbegin(), mTmpSignal2.rend(), first, std::ptr_fun(RealTypeTraits<real_t>::toInt16));
}

template <typename Iter>
template <typename InputIter, typename OutputIter>
void DigitalAudioFilterImpl<Iter>::filterSignal(InputIter first,
                                                InputIter last,
                                                OutputIter dst)
{
    const real_t zero = 0;
    // fill first bytes with zeros
    std::fill(dst, dst + filter_size, zero);
    last -= filter_size;

    auto& a = filter_denum;
    auto& b = filter_num;

    for (; first != last; ++first, ++dst) {
        auto x = std::inner_product(b.begin(), b.end(), first, zero);
        auto y = std::inner_product(a.begin(), a.end(), dst, zero);
        dst[filter_size - 1] = x - y;
    }

    // fill last bytes with original samples
    std::copy(first, first + filter_size, dst);
}

}


