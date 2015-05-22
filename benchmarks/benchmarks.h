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

typedef std::pair<msec, double> benchmark_result;

typedef std::shared_ptr<trikSound::AngleDetector<Iter>>       AngleDetectorPtr;
typedef std::shared_ptr<trikSound::StereoVadFilter<Iter>>     VadFilterPtr;
typedef std::shared_ptr<trikSound::DigitalAudioFilter<Iter>>  DigitalAudioFilterPtr;

class BenchmarkWorker
{
public:

    static const int ITERATION_COUNT                = 100;
    static const trikSound::sample_type SAMPLE_MAX  = std::numeric_limits<trikSound::sample_type>::max();

    BenchmarkWorker();

    int getWindowSize() const;
    void setWindowSize(int windowSize);

    benchmark_result benchmarkAngleDetector(const AngleDetectorPtr& detector);

    benchmark_result benchmarkVadFilter(const VadFilterPtr& vad);

    benchmark_result benchmarkDigitalFilter(const DigitalAudioFilterPtr& filter);

private:

    typedef trikSound::StereoAudioFilter<Iter>::range_type range_type;

    template <typename Callback>
    benchmark_result do_benchmark(const Callback& cb);

    void generate_random(Iter first, Iter last);

    int mWindowSize;
    std::random_device mRd;
    std::default_random_engine mGen;
};

