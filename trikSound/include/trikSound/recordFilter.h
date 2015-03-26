#pragma once

#include <iterator>
#include <vector>

#include <QString>

#include "triksound_global.h"
#include "audioFilter.h"
#include "wavFile.h"

namespace trikSound
{

template <typename Iter>
class RecordFilter: public AudioFilter<Iter>
{
public:

    using AudioFilter<Iter>::FilterPtr;

    RecordFilter(const QString& filename, const FilterPtr& prevFilter);

private:

    void handleWindowImpl(Iter first, Iter last);

private:
    WavFile mWavFile;
};

template <typename Iter>
RecordFilter<Iter>::RecordFilter(const QString& filename, const AudioFilter::FilterPtr& prevFilter):
    AudioFilter(prevFilter)
  , mWavFile(filename)
{
    mWavFile.open(WavFile::WriteOnly);
}

template <typename Iter>
void RecordFilter<Iter>::handleWindowImpl(Iter first, Iter last)
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    std::vector<value_type> buf(first, last);
    mWavFile.write((char*)buf.data(), buf.size() * sizeof(value_type));
}

}
