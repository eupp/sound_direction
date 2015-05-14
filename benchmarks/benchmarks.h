#pragma once

#include <memory>
#include <vector>

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

msec benchmarkAngleDetector(const AngleDetectorPtr& detector);

msec benchmarkVadFilter(const VadFilterPtr& vad);

msec benchmarkDigitalFilter(const DigitalAudioFilterPtr& filter);
