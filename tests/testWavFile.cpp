#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <random>

#include "trikSound/wavFile.h"
#include "trikSound/types.h"

using namespace std;
using namespace boost;
using namespace trikSound;

class WavFileFixture
{
public:

    WavFileFixture();

protected:

    static const size_t sample_type_size = sizeof(sample_type);
    static const size_t arraySize = 8 * WavFileFixture::sample_type_size;

    WavFile file;
    QAudioFormat format;
    array<char, arraySize> randBytes;
};

WavFileFixture::WavFileFixture():
    file("autotest.wav")
{
    format.setChannels(1);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(8000);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(8 * sample_type_size);
    format.setCodec("audio/pcm");

    random_device rd;
    default_random_engine gen(rd());
    generate(randBytes.begin(), randBytes.end(), [&gen]() {return (char) gen() % 256; });
}

BOOST_TEST_DONT_PRINT_LOG_VALUE(QAudioFormat)

BOOST_FIXTURE_TEST_SUITE(testWavFile, WavFileFixture)

BOOST_AUTO_TEST_CASE(testAudioFormat)
{
    file.open(WavFile::WriteOnly, format);
    BOOST_CHECK(file.isOpen());

    file.open(WavFile::ReadOnly);
    BOOST_CHECK(file.isOpen());

    QAudioFormat readFormat = file.audioFormat();
    BOOST_CHECK_EQUAL(readFormat, format);
}

BOOST_AUTO_TEST_CASE(testWrite)
{
    int t= format.sampleSize();
    file.open(WavFile::WriteOnly, format);
    qint64 bytesWritten = file.write(randBytes.data(), randBytes.size());

    BOOST_CHECK_EQUAL(bytesWritten, randBytes.size());
}

BOOST_AUTO_TEST_CASE(testSize)
{
    file.open(WavFile::WriteOnly, format);
    qint64 bytesWritten = file.write(randBytes.data(), randBytes.size());

    BOOST_CHECK_EQUAL(file.size() - WavFile::HEADER_SIZE, randBytes.size());
    BOOST_CHECK_EQUAL(file.sampleCount(), randBytes.size() / sample_type_size);
}

BOOST_AUTO_TEST_CASE(testRead)
{
    file.open(WavFile::WriteOnly, format);
    qint64 bytesWritten = file.write(randBytes.data(), randBytes.size());

    file.open(WavFile::ReadOnly, format);
    AudioBuffer buf = file.readAll();

    BOOST_CHECK_EQUAL(buf.audioFormat(), format);
    BOOST_CHECK_EQUAL(buf.size(), randBytes.size());
    BOOST_CHECK_EQUAL_COLLECTIONS((char*)buf.data(), (char*)buf.data() + buf.size(),
                                  randBytes.begin(), randBytes.end());
}

BOOST_AUTO_TEST_SUITE_END()
