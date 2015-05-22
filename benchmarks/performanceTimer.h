#pragma once

#include <ctime>
#include <cstdint>

class PerformanceTimer
{
public:

    typedef double msec;

    PerformanceTimer():
        mStart(clock())
    {}

    msec elapses_msec()
    {
        return (double) (clock() - mStart) / TO_MILLY_SECONDS_COEF;
    }

private:

    static const int TO_MILLY_SECONDS_COEF = CLOCKS_PER_SEC / 1000;

     clock_t mStart;
};
