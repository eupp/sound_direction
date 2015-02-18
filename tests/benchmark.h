#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <ostream>

#include <QString>

void testWindowHandler(const QString& filename, std::ostream& out, size_t duration = 0);

qint32 angleDetectorBenchmark(const QString& filename, size_t frameSize);

#endif // BENCHMARK_H

