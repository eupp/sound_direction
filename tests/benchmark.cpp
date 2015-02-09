#include "tests/benchmark.h"

#include <ctime>

#include <QDebug>

#include "include/internal/angleDetector.h"
#include "include/internal/audioBuffer.h"
#include "include/internal/wavfile.h"
#include "include/internal/utils.h"
#include "include/internal/digitalAudioFilter.h"

qint32 angleDetectorBenchmark(const QString& filename, size_t frameSize)
{
    WavFile file(filename);
    file.open(WavFile::ReadOnly);
    qint64 bytesCount = bytesForDuration(frameSize, file.audioFormat());
    AudioBuffer buf = file.read(bytesCount);
    AudioBuffer chl1 = buf.leftChannel();
    AudioBuffer chl2 = buf.rightChannel();
    DigitalAudioFilter filter;
    AngleDetector detector;

    qDebug() << "Bytes count in frame: " <<  bytesCount;

    const double micrDist = 10.0;
    const int iterCount = 100;

    clock_t c1 = clock();
    for (int i = 0; i < iterCount; ++i) {
        AudioBuffer filt1 = filter.input(chl1);
        AudioBuffer filt2 = filter.input(chl2);
        detector.getAngle(filt1, filt2, micrDist);
    }
    clock_t c2 = clock();

//    qDebug() << CLOCKS_PER_SEC;
    qDebug() << c2 - c1;

    return ((c2 - c1) * 1000) / (CLOCKS_PER_SEC * iterCount);
}
