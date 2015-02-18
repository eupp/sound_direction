#include "tests/benchmark.h"

#include <utility>
#include <vector>
#include <ctime>
#include <iomanip>

#include <QDebug>

#include "include/internal/angleDetector.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/wavfile.h"
#include "include/internal/utils.h"
#include "include/internal/debugUtils.h"
#include "include/internal/digitalAudioFilter.h"

using namespace std;

typedef vector<sample_t> sample_vector;

pair<sample_vector, sample_vector> readWavToVector(WavFile& file, qint64 samplesNum = -1)
{
    if (samplesNum == -1) {
        samplesNum = file.sampleCount();
    }

    AudioBuffer buf = file.read(samplesNum);
    AudioBuffer chl1 = buf.leftChannel();
    AudioBuffer chl2 = buf.rightChannel();

    sample_vector svector1(chl1.sampleCount());
    sample_vector svector2(chl2.sampleCount());

    std::copy((sample_t*) chl1.data(), (sample_t*) chl1.data() + chl1.sampleCount(), svector1.begin());
    std::copy((sample_t*) chl2.data(), (sample_t*) chl2.data() + chl2.sampleCount(), svector2.begin());

    return make_pair(svector1, svector2);
}

qint32 angleDetectorBenchmark(const QString& filename, size_t frameSize)
{
    WavFile file(filename);
    file.open(WavFile::ReadOnly);
    QAudioFormat fmt = file.audioFormat();
    qint64 samplesCount = samplesForDuration(frameSize, fmt);

    auto vectors = readWavToVector(file, samplesCount);
    sample_vector& chl1 = vectors.first;
    sample_vector& chl2 = vectors.second;

    DigitalAudioFilter<sample_vector> filter;
    AngleDetector<sample_vector> detector(fmt, 10.2);

//    qDebug() << "Bytes count in frame: " <<  bytesCount;

    const int iterCount = 100;

    clock_t c1 = clock();
    for (int i = 0; i < iterCount; ++i) {
        filter.handleWindow(chl1.begin(), chl1.end());
        filter.handleWindow(chl2.begin(), chl2.end());
        detector.handleWindow(chl1.begin(), chl1.end(), chl2.begin(), chl2.end());
        double angle = detector.getAngle();
    }
    clock_t c2 = clock();

    qDebug() << c2 - c1;

    return ((c2 - c1) * 1000) / (CLOCKS_PER_SEC * iterCount);
}


void testWindowHandler(const QString& filename, std::ostream& out, size_t duration)
{
    WavFile file(filename);
    file.open(WavFile::ReadOnly);
    QAudioFormat fmt = file.audioFormat();

    qint64 samplesCount = duration ? samplesForDuration(duration, fmt) : (file.sampleCount() - 1000);

    const size_t offset = 1000;
    if (file.sampleCount() < offset + samplesCount) {
        qDebug() << "File is too short";
    }
    file.seek(offset);


    auto vectors = readWavToVector(file, file.sampleCount());
    sample_vector& chl1 = vectors.first;
    sample_vector& chl2 = vectors.second;

    dprint_sequence("channel1.test", chl1.begin(), chl1.end());
    dprint_sequence("channel2.test", chl2.begin(), chl2.end());

    DigitalAudioFilter<sample_vector> filter;
    AngleDetector<sample_vector> detector(fmt, 10.2);

    filter.handleWindow(chl1.begin(), chl1.end());
    filter.handleWindow(chl2.begin(), chl2.end());

    dprint_sequence("filt1.test", chl1.begin(), chl1.end());
    dprint_sequence("filt2.test", chl2.begin(), chl2.end());

    const int rpadd = 100;

    chl1.resize(chl1.size() - rpadd);
    chl2.resize(chl2.size() - rpadd);

    detector.handleWindow(chl1.begin(), chl1.end(), chl2.begin(), chl2.end());
    double angle = detector.getAngle();

    out << "Angle (whole file): " << fixed << setprecision(4) << angle << endl;

    AngleDetector<sample_vector> windowDetector(fmt, 10.2);

    int signalSize = chl1.size();
    const int windowSize = 512;

    out << endl << "Window size: " << windowSize << endl << endl;

    auto u = chl1.begin();
    auto v = chl2.begin();
    int windowNum = 1;
    while (windowSize <= signalSize) {
        windowDetector.handleWindow(u, u + windowSize, v, v + windowSize);
        double angle = windowDetector.getAngle();

        out << "window #" << windowNum << "; hypothesis: " << angle << endl;

        ++windowNum;
        signalSize -= windowSize;
        u += windowSize;
        v += windowSize;
    }
}
