#include "benchmarks.h"

#include <limits>
#include <array>
#include <algorithm>

using namespace std;
using namespace trikSound;

BenchmarkWorker::BenchmarkWorker():
    mWindowSize(0)
  , mRd()
  , mGen(mRd())
{}

int BenchmarkWorker::getWindowSize() const
{
    return mWindowSize;
}

void BenchmarkWorker::setWindowSize(int windowSize)
{
    mWindowSize = windowSize;
}

benchmark_result BenchmarkWorker::benchmarkAngleDetector(const AngleDetectorPtr& detector)
{

    auto cb = [&detector](range_type chl1, range_type chl2) -> void
    {
        detector->handleWindow(chl1, chl2);
    };
    return do_benchmark(cb);
}

benchmark_result BenchmarkWorker::benchmarkVadFilter(const VadFilterPtr& vad)
{
    auto cb = [&vad](range_type chl1, range_type chl2) -> void
    {
        vad->handleWindow(chl1, chl2);
    };
    return do_benchmark(cb);
}

benchmark_result BenchmarkWorker::benchmarkDigitalFilter(const DigitalAudioFilterPtr& filter)
{
    auto cb = [&filter](range_type chl1, range_type chl2) -> void
    {
        filter->handleWindow(chl1.first, chl1.second);
        filter->handleWindow(chl2.first, chl2.second);
    };
    return do_benchmark(cb);
}

void BenchmarkWorker::generate_random(Iter first, Iter last)
{
    generate(first, last, [this]() {return (sample_type) mGen() % SAMPLE_MAX; });
}

template <typename Callback>
benchmark_result BenchmarkWorker::do_benchmark(const Callback& cb)
{
    const int asize = 2;
    array<Container, asize> data;
    data[0].resize(2 * mWindowSize);
    data[1].resize(2 * mWindowSize);
    generate_random(data[0].begin(), data[0].end());
    generate_random(data[1].begin(), data[1].end());

    Iter first;
    Iter sep;
    Iter last;

    array<double, ITERATION_COUNT> ts = {0};
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        first   = data[i % asize].begin();
        sep     = first + mWindowSize;
        last    = data[i % asize].end();

        int count = 100;

        PerformanceTimer timer;
        for (int j = 0; j < count; ++j) {
            cb(make_pair(first, sep), make_pair(sep, last));
        }
        ts[i] = timer.elapses_msec() / count;
    }

    double total = accumulate(ts.begin(), ts.end(), 0);
    double avr = total / ITERATION_COUNT;
    double dev = 0;
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        double d = ts[i] - avr;
        d *= d;
        dev += d;
    }
    dev /= ITERATION_COUNT;
    dev = sqrt(dev);

    return make_pair(round(total), dev);
}
