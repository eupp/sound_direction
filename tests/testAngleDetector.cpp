#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "trikSound/angleDetector.h"
#include "trikSound/splitFilter.h"
#include "trikSound/digitalAudioFilter.h"
#include "trikSound/wavFile.h"

using namespace std;
using namespace boost;
using namespace trikSound;

class AngleDetectorFixture
{
public:

    typedef sample_type* iterator_type;

    AngleDetectorFixture();

    void readAudioData(const QString& filename);

protected:

    std::shared_ptr<AngleDetector<iterator_type>> detector;
    AngleDetector<iterator_type>::range_type channel1;
    AngleDetector<iterator_type>::range_type channel2;
    QAudioFormat format;

    AudioBuffer bufl;
    AudioBuffer bufr;

    const int ord = 100;
};


AngleDetectorFixture::AngleDetectorFixture():
    detector(new AngleDetector<iterator_type>())
{
    std::shared_ptr<AudioFilter<iterator_type>> filter =
            make_shared<DigitalAudioFilter<iterator_type>>();
    std::shared_ptr<StereoAudioFilter<iterator_type>> split =
            make_shared<SplitFilter<iterator_type>>(filter);

    detector->insertFilter(split);
    detector->setMicrDist(10.2);
}

void AngleDetectorFixture::readAudioData(const QString& filename)
{
    WavFile file(filename);
    file.open(WavFile::ReadOnly);

    format = file.audioFormat();
    detector->setSampleRate(format.sampleRate());
    const size_t offset = 1000;
    file.seek(offset);
    AudioBuffer buf = file.readAll();

    bufl = buf.leftChannel();
    bufr = buf.rightChannel();

    channel1.first = (sample_type*) bufl.data();
    channel1.second = (sample_type*) bufl.data() + bufl.sampleCount();

    channel2.first = (sample_type*) bufr.data();
    channel2.second = (sample_type*) bufr.data() + bufr.sampleCount();
}

BOOST_FIXTURE_TEST_SUITE(testAngleDetector, AngleDetectorFixture)

BOOST_AUTO_TEST_CASE(testLeft)
{
    readAudioData("wav/left.wav");
    detector->handleWindow(channel1, channel2);
    int angle = round(ord * detector->getAngle());
    BOOST_CHECK_EQUAL(angle, -90 * ord);
}

BOOST_AUTO_TEST_CASE(testCenter)
{
    readAudioData("wav/center.wav");
    detector->handleWindow(channel1, channel2);
    int angle = round(ord * detector->getAngle());
    BOOST_CHECK_EQUAL(angle, 0 * ord);
}

BOOST_AUTO_TEST_CASE(testRight)
{
    readAudioData("wav/right.wav");
    detector->handleWindow(channel1, channel2);
    int angle = round(ord * detector->getAngle());
    BOOST_CHECK_EQUAL(angle, 90 * ord);
}

BOOST_AUTO_TEST_SUITE_END()
