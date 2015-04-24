#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>

#include "trikSound/circularBuffer.h"
#include "trikSound/singleChannelCircularBuffer.h"
#include "trikSound/doubleChannelCircularBuffer.h"
#include "trikSound/circularBufferQAdapter.h"

using namespace std;
using namespace boost;
using namespace trikSound;

class CircularBufferFixture
{
public:
    CircularBufferFixture();

protected:
    static const size_t sample_type_size = sizeof(sample_type);
    static const size_t capacity = 8 * CircularBufferFixture::sample_type_size;

    typedef array<char, 3 * capacity> test_array;

    test_array randBytes;
    test_array readBytes;
    std::shared_ptr<CircularBuffer> cbSingle;
    std::shared_ptr<CircularBuffer> cbDouble;
    CircularBufferQAdapter cb;
};

const size_t CircularBufferFixture::sample_type_size;
const size_t CircularBufferFixture::capacity;

CircularBufferFixture::CircularBufferFixture():
    cbSingle(make_shared<SingleChannelCircularBuffer>(capacity / sample_type_size))
  , cbDouble(make_shared<DoubleChannelCircularBuffer>(capacity / sample_type_size))
  , cb(cbSingle)
{
    cb.open(QIODevice::ReadWrite);

    random_device rd;
    default_random_engine gen(rd());
    generate(randBytes.begin(), randBytes.end(), [&gen]() {return (char) gen() % 256; });
    fill(readBytes.begin(), readBytes.end(), 0);
}

BOOST_FIXTURE_TEST_SUITE(testCircularBufferQAdaptor, CircularBufferFixture)

BOOST_AUTO_TEST_CASE(testSimpleWrite)
{
    size_t bytesCount = capacity / 2;
    cb.write(randBytes.data(), bytesCount);

    qint64 readCount = cb.read(readBytes.data(), bytesCount);

    BOOST_CHECK_EQUAL(readCount, bytesCount);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + bytesCount,
                                  randBytes.data(),
                                  randBytes.data() + bytesCount);
}

BOOST_AUTO_TEST_CASE(bytesAvailableTest)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);

    BOOST_CHECK_EQUAL(cb.bytesAvailable(), capacity);

    cb.read(readBytes.data(), halfCapacity);

    BOOST_CHECK_EQUAL(cb.bytesAvailable(), halfCapacity);
}

BOOST_AUTO_TEST_CASE(testOverwrite)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);

    // overrun first half of buffer
    cb.write(randBytes.data() + capacity, halfCapacity);

    qint64 readCount = cb.read(readBytes.data(), halfCapacity);

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + halfCapacity,
                                  randBytes.data() + halfCapacity,
                                  randBytes.data() + capacity);

    readCount = cb.read(readBytes.data(), halfCapacity);

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + halfCapacity,
                                  randBytes.data() + capacity,
                                  randBytes.data() + 3 * halfCapacity);
}

BOOST_AUTO_TEST_CASE(testReadWrite)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), halfCapacity);

    // read all available samples except one
    cb.read(halfCapacity - sample_type_size);

    // overwrite whole buffer except one unread sample
    cb.write(randBytes.data(), capacity - sample_type_size);

    qint64 readCount = cb.read(readBytes.data(), sample_type_size);

    BOOST_CHECK_EQUAL(readCount, sample_type_size);

    // check that first sample is that unread sample from first write operation
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + sample_type_size,
                                  randBytes.data() + halfCapacity - sample_type_size,
                                  randBytes.data() + halfCapacity);
}

BOOST_AUTO_TEST_CASE(testReadWriteOverwrite)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);

    // read all available samples except one
    cb.read(halfCapacity - sample_type_size);

    // overwrite whole buffer including that lonely sample
    cb.write(randBytes.data(), capacity);

    qint64 readCount = cb.read(readBytes.data(), sample_type_size);

    BOOST_CHECK_EQUAL(readCount, sample_type_size);
    // in this case circular buffer should redirect read position
    // to its beginning
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + sample_type_size,
                                  randBytes.data(),
                                  randBytes.data() + sample_type_size);
}

BOOST_AUTO_TEST_CASE(testSimpleWriteDouble)
{
    cb.setCircularBuffer(cbDouble);
    cb.open(QIODevice::ReadWrite);

    const size_t bytesCount = capacity / 2;
    cb.write(randBytes.data(), bytesCount);

    const int channelSize = bytesCount / 4;
    sample_type* sample_data = (sample_type*) randBytes.data();
    array<sample_type, channelSize> rearranged;
    for (int i = 0; i < channelSize; ++i) {
        rearranged[i] = sample_data[2 * i];
        rearranged[i + channelSize] = sample_data[2 * i + 1];
    }

    qint64 readCount = cb.read(readBytes.data(), bytesCount);

    BOOST_CHECK_EQUAL(readCount, bytesCount);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + bytesCount,
                                  (char*)rearranged.data(),
                                  (char*)rearranged.data() + bytesCount);
}

BOOST_AUTO_TEST_CASE(testOverwriteDouble)
{
    cb.setCircularBuffer(cbDouble);
    cb.open(QIODevice::ReadWrite);

    const size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);

    // overrun first half of buffer
    cb.write(randBytes.data() + capacity, halfCapacity);

    // check first half of readed samples
    // (which should be equal to the second half of rand bytes)

    qint64 readCount = cb.read(readBytes.data(), halfCapacity);

    const int channelSize = halfCapacity / 4;
    array<sample_type, channelSize> rearranged;
    sample_type* sample_data = (sample_type*) (randBytes.data() + halfCapacity);
    for (int i = 0; i < channelSize; ++i) {
        rearranged[i] = sample_data[2 * i];
        rearranged[i + channelSize] = sample_data[2 * i + 1];
    }

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + halfCapacity,
                                  (char*) rearranged.data(),
                                  (char*) rearranged.data() + halfCapacity);

    // check second half of readed samples
    // (which should be equal to the second half of rand bytes)

    readCount = cb.read(readBytes.data(), halfCapacity);

    sample_data = (sample_type*) (randBytes.data() + capacity);
    for (int i = 0; i < channelSize; ++i) {
        rearranged[i] = sample_data[2 * i];
        rearranged[i + channelSize] = sample_data[2 * i + 1];
    }

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + halfCapacity,
                                  (char*) rearranged.data(),
                                  (char*) rearranged.data() + halfCapacity);
}

BOOST_AUTO_TEST_SUITE_END()
