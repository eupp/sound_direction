#ifndef AUDIO_FILTER_H
#define AUDIO_FILTER_H

#include <array>

#include <QDebug>

#include "include/internal/types.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/iAudioFilter.h"
#include "include/internal/realTypeTraits.h"

class DigitalAudioFilter: public QObject, public IAudioFilter
{
    Q_OBJECT
public:
    DigitalAudioFilter()
    {
        std::reverse(filter_denum.begin(), filter_denum.end());
        std::reverse(filter_num.begin(), filter_num.end());
    }

signals:
    void output(AudioBuffer buf);
public slots:
    AudioBuffer input(const AudioBuffer& buf);
private:
    const static int filter_size = 7;

//    typedef std:: isignal_type;
    typedef std::vector<real_t> fsignal_type;
    typedef std::array<real_t, filter_size> filter_coeffs;

    template <typename T, typename U>
    void filtfilt(T first,
                  T last,
                  U dst)
    {
        const size_t n = last - first;

        static fsignal_type y_h1(n);
        static fsignal_type y_h2(n);
        y_h1.resize(n);
        y_h2.resize(n);

        filter(first, last, y_h1.begin());
        std::reverse(y_h1.begin(), y_h1.end());
        filter(y_h1.begin(), y_h1.end(), y_h2.begin());

        std::transform(y_h2.rbegin(), y_h2.rend(), dst, std::ptr_fun(RealTypeTraits<real_t>::toInt16));
    }

    template <typename in_T, typename out_T>
    void filter(in_T first,
                in_T last,
                out_T dst)
    {
        const real_t zero = 0;
        std::fill(dst, dst + filter_size, zero);
//        dst += filter_size;
        last -= filter_size;
        for (; first != last; ++first, ++dst) {

            auto& a = filter_denum;
            auto& b = filter_num;

            auto x = std::inner_product(b.begin(), b.end(), first, zero);
            auto y = std::inner_product(a.begin(), a.end(), dst, zero);
            dst[filter_size - 1] = x - y;
        }
        std::copy(first, first + filter_size, dst);
    }

    filter_coeffs filter_num = {{0.003281, 0.0064564, -0.0032273, -0.0129112, -0.0032273, 0.0064564, 0.003281}};
    filter_coeffs filter_denum = {{0, -4.4221, 8.2622, -8.3659, 4.8404, -1.5142, 0.1999}};
};


#endif // FILTER_H
