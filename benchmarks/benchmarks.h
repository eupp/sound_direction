#pragma once

#include <memory>
#include <vector>
#include <random>

#include "trikSound/types.h"
#include "trikSound/angleDetector.h"
#include "trikSound/stereoVadFilter.h"
#include "trikSound/digitalAudioFilter.h"

#include "performanceTimer.h"

typedef PerformanceTimer::msec msec;

typedef std::vector<trikSound::sample_type> Container;
typedef Container::iterator                 Iter;

typedef std::shared_ptr<trikSound::AngleDetector<Iter>>       AngleDetectorPtr;
typedef std::shared_ptr<trikSound::StereoVadFilter<Iter>>     VadFilterPtr;
typedef std::shared_ptr<trikSound::DigitalAudioFilter<Iter>>  DigitalAudioFilterPtr;

class BenchmarkWorker
{
public:

    static const int ITERATION_COUNT                = 1000;
    static const trikSound::sample_type SAMPLE_MAX  = std::numeric_limits<trikSound::sample_type>::max();

    BenchmarkWorker();

    int getWindowSize() const;
    void setWindowSize(int windowSize);

    msec benchmarkAngleDetector(const AngleDetectorPtr& detector);

    msec benchmarkVadFilter(const VadFilterPtr& vad);

    msec benchmarkDigitalFilter(const DigitalAudioFilterPtr& filter);

private:

    typedef trikSound::StereoAudioFilter<Iter>::range_type range_type;

    template <typename Callback>
    msec do_benchmark(const Callback& cb);

    void generate_random(Iter first, Iter last);

    int mWindowSize;
    std::random_device mRd;
    std::default_random_engine mGen;
};

