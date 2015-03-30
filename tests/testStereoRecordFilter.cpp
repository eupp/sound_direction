#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <random>

#include "trikSound/stereoRecordFilter.h"
#include "trikSound/types.h"

using namespace std;
using namespace boost;
using namespace trikSound;

class StereoRecordFilterFixture
{
public:

    StereoRecordFilterFixture();

protected:

    static QAudioFormat getFormat();

    static const size_t sample_type_size = sizeof(sample_type);
    static const size_t arraySize = 8;

    typedef typename array<sample_type, arraySize>::iterator iterator_type;
    typedef typename StereoRecordFilter<iterator_type>::range_type range_type;

    QAudioFormat format;
    StereoRecordFilter<iterator_type>::WavFilePtr wavFile;
    StereoRecordFilter<iterator_type>::FilterPtr filter;
    array<sample_type, arraySize> randBytes;
};



StereoRecordFilterFixture::StereoRecordFilterFixture():
    wavFile(make_shared<WavFile>("autotest.wav"))
{
    format.setChannels(1);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(8000);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(8 * sample_type_size);
    format.setCodec("audio/pcm");

    wavFile->open(WavFile::WriteOnly, format);
    filter = make_shared<StereoRecordFilter<iterator_type>>(wavFile);

    random_device rd;
    default_random_engine gen(rd());
    generate(randBytes.begin(), randBytes.end(), [&gen]() {return (char) gen() % 1000; });
}

BOOST_FIXTURE_TEST_SUITE(testStereoRecordFilter, StereoRecordFilterFixture)

BOOST_AUTO_TEST_CASE(testHandleWindow)
{
    array<sample_type, arraySize / 2> channel1;
    array<sample_type, arraySize / 2> channel2;
    for (size_t i = 0; i < arraySize / 2; ++i) {
        channel1[i] = randBytes[2 * i];
        channel2[i] = randBytes[2 * i + 1];
    }
    range_type range1 = make_pair(channel1.begin(), channel1.end());
    range_type range2 = make_pair(channel2.begin(), channel2.end());
    filter->handleWindow(range1, range2);

    wavFile->close();
    wavFile->open(WavFile::ReadOnly);

    AudioBuffer buf = wavFile->readAll();

    BOOST_CHECK_EQUAL(buf.sampleCount(), randBytes.size());
    BOOST_CHECK_EQUAL_COLLECTIONS((sample_type*)buf.data(), (sample_type*)buf.data() + buf.sampleCount(),
                                  randBytes.begin(), randBytes.end());
}

BOOST_AUTO_TEST_SUITE_END()
