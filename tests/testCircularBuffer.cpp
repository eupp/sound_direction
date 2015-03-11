#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>

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

    array<char, 3 * capacity> randBytes;
    array<char, 3 * capacity> readBytes;
    std::shared_ptr<circular_buffer<sample_type>> cbInner;
    CircularBufferQAdapter cb;
};

const size_t CircularBufferFixture::sample_type_size;
const size_t CircularBufferFixture::capacity;

CircularBufferFixture::CircularBufferFixture():
    cbInner(make_shared<circular_buffer<sample_type>>(capacity / sample_type_size))
  , cb(cbInner)
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
    cb.read(halfCapacity - sample_type_size);
    cb.write(randBytes.data(), capacity - sample_type_size);

    qint64 readCount = cb.read(readBytes.data(), sample_type_size);

    BOOST_CHECK_EQUAL(readCount, sample_type_size);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + sample_type_size,
                                  randBytes.data() + halfCapacity - sample_type_size,
                                  randBytes.data() + halfCapacity);
}

BOOST_AUTO_TEST_CASE(testReadWriteOverwrite)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);
    cb.read(halfCapacity - sample_type_size);
    cb.write(randBytes.data(), capacity);

    qint64 readCount = cb.read(readBytes.data(), sample_type_size);

    BOOST_CHECK_EQUAL(readCount, sample_type_size);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(),
                                  readBytes.data() + sample_type_size,
                                  randBytes.data(),
                                  randBytes.data() + sample_type_size);
}

BOOST_AUTO_TEST_SUITE_END()
