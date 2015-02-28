#pragma once

#include <array>
#include <vector>

#include "../trikSound/types.h"
#include "realTypeTraits.h"

namespace trikSound {

template <typename C>
class DigitalAudioFilterImpl
{
public:

    typedef typename C::iterator iterator_type;
    typedef typename C::const_iterator const_iterator_type;

    DigitalAudioFilterImpl();

    void handleWindow(iterator_type first, iterator_type last);

private:

    const static int filter_size = 7;

    typedef std::vector<real_t> fsignal_type;
    typedef std::array<real_t, filter_size> filter_coeffs;

    filter_coeffs filter_num = {{0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281}};
    filter_coeffs filter_denum = {{0, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999}};

    void filtfilt(iterator_type first, iterator_type last);

    template <typename T, typename U>
    void filter(typename T::const_iterator first, typename T::const_iterator last, typename U::iterator dst);
};

template <typename C>
DigitalAudioFilterImpl<C>::DigitalAudioFilterImpl()
{
    std::reverse(filter_denum.begin(), filter_denum.end());
    std::reverse(filter_num.begin(), filter_num.end());
}

template <typename C>
void DigitalAudioFilterImpl<C>::handleWindow(iterator_type first,
                                             iterator_type last)
{
    filtfilt(first, last);
}

template <typename C>
void DigitalAudioFilterImpl<C>::filtfilt(iterator_type first, iterator_type last)
{
    const size_t n = last - first;

    static fsignal_type y_h1(n);
    static fsignal_type y_h2(n);
    y_h1.resize(n);
    y_h2.resize(n);

    filter<C, fsignal_type>(first, last, y_h1.begin());
    std::reverse(y_h1.begin(), y_h1.end());
    filter<fsignal_type, fsignal_type>(y_h1.begin(), y_h1.end(), y_h2.begin());

    std::transform(y_h2.rbegin(), y_h2.rend(), first, std::ptr_fun(RealTypeTraits<real_t>::toInt16));
}

template <typename C>
template <typename T, typename U>
void DigitalAudioFilterImpl<C>::filter(typename T::const_iterator first,
                                       typename T::const_iterator last,
                                       typename U::iterator dst)
{
    const real_t zero = 0;
    std::fill(dst, dst + filter_size, zero);
    last -= filter_size;

    auto& a = filter_denum;
    auto& b = filter_num;

    for (; first != last; ++first, ++dst) {
        auto x = std::inner_product(b.begin(), b.end(), first, zero);
        auto y = std::inner_product(a.begin(), a.end(), dst, zero);
        dst[filter_size - 1] = x - y;
    }

    std::copy(first, first + filter_size, dst);
}

}


