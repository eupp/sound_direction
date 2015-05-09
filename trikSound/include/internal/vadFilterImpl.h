#pragma once

#include <iterator>
#include <limits>

#include "vadFilter.h"

namespace trikSound
{

template <typename Iter>
class VadFilterImpl
{
private:

    typedef unsigned long long energy_type;

public:

    typedef typename std::iterator_traits<Iter>::value_type value_type;

    VadFilterImpl():
        mEnrgCoef(0)
    {}

    void handleWindowImpl(Iter first, Iter last)
    {
        energy_type enrg = 0;
        for (; first != last; ++first) {
            energy_type x = static_cast<energy_type>(*first);
            enrg += x * x;
        }

        energy_type frameMax = SQR_MAX * (last - first);

        mEnrgCoef = enrg / frameMax;
    }

    double getEnergyCoefficient() const
    {
        return mEnrgCoef;
    }

private:

    static const energy_type SQR_MAX = static_cast<energy_type>(std::numeric_limits<value_type>::max())
                                     * static_cast<energy_type>(std::numeric_limits<value_type>::max());


    double mEnrgCoef;

};

}
