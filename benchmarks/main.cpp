#include <iostream>
#include <fstream>
#include <array>

#include "trikSound/types.h"
#include "trikSound/angleDetector.h"
#include "trikSound/stereoVadFilter.h"
#include "trikSound/digitalAudioFilter.h"

#include "benchmarks.h"

using namespace std;
using namespace trikSound;


int main()
{
    ofstream fout;
    fout.open("benchmarks.out");
    ostream& out = fout;

    const int sampleRate = 44100;
    const int micrDist = 7.6;
    const int historyDepth = 20;
    const double threshold = 0.0001;

    DigitalAudioFilterPtr filter    = make_shared<DigitalAudioFilter<Iter>>();
    AngleDetectorPtr detector       = make_shared<AngleDetector<Iter>>(sampleRate, micrDist, historyDepth);
    VadFilterPtr vad                = make_shared<StereoVadFilter<Iter>>(threshold);

    BenchmarkWorker worker;
    array<int, 4> wSizes = {256, 512, 1024, 2048};

    out << "Benchmarks: iteration count = " << BenchmarkWorker::ITERATION_COUNT << endl;

    for (int i = 0; i < wSizes.size(); ++i) {

        out << "Benchmarks: window size = " << wSizes[i] << endl;

        worker.setWindowSize(wSizes[i]);
        msec res = 0;

        res = worker.benchmarkAngleDetector(detector);
        out << "Angle detector benchmark " << endl;
        out << "total ms = " << res << endl;
        out << "average ms = " << (double) res / BenchmarkWorker::ITERATION_COUNT << endl;
        out << endl;

        res = worker.benchmarkVadFilter(vad);
        out << "Vad benchmark " << endl;
        out << "total ms = " << res << endl;
        out << "average ms = " << (double) res / BenchmarkWorker::ITERATION_COUNT << endl;
        out << endl;

        res = worker.benchmarkDigitalFilter(filter);
        out << "Filter benchmark " << endl;
        out << "total ms = " << res << endl;
        out << "average ms = " << (double) res / BenchmarkWorker::ITERATION_COUNT << endl;
        out << endl;
    }

    return 0;
}

