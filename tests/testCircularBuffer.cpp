#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <array>
#include <chrono>
#include <random>
#include <algorithm>

#include "trikSound/circularBufferQAdapter.h"

using namespace std;
using namespace trikSound;

class CircularBufferFixture
{
public:

    CircularBufferFixture();

protected:
    static const size_t capacity = 10;
    array<char, 3 * capacity> randBytes;
    array<char, 3 * capacity> readBytes;
    CircularBufferQAdapter cb;
};


CircularBufferFixture::CircularBufferFixture():
    cb(capacity)
{
    cb.open(QIODevice::ReadWrite);

    random_device rd;
    default_random_engine gen(rd());
    generate(randBytes.begin(), randBytes.end(), [&gen]() {return (char) gen() % 256; });
    fill(readBytes.begin(), readBytes.end(), 0);

    cout << "setUp fixture" << endl;
}

BOOST_FIXTURE_TEST_SUITE(testCircularBufferQAdaptor, CircularBufferFixture)

BOOST_AUTO_TEST_CASE(testSimpleWrite)
{
    size_t bytesCount = capacity / 2;
    cb.write(randBytes.data(), bytesCount);

    qint64 readCount = cb.read(readBytes.data(), bytesCount);

    BOOST_CHECK_EQUAL(readCount, bytesCount);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(), readBytes.data() + bytesCount,
                                  randBytes.data(), randBytes.data() + bytesCount);
}

BOOST_AUTO_TEST_CASE(testOverrunWrite)
{
    size_t halfCapacity = capacity / 2;
    cb.write(randBytes.data(), capacity);
    // overrun first half of buffer
    cb.write(randBytes.data() + capacity, halfCapacity);

    qint64 readCount = cb.read(readBytes.data(), halfCapacity);

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(), readBytes.data() + halfCapacity,
                                  randBytes.data() + capacity, randBytes.data() + 3 * halfCapacity);

    readCount = cb.read(readBytes.data(), halfCapacity);

    BOOST_CHECK_EQUAL(readCount, halfCapacity);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(), readBytes.data() + halfCapacity,
                                  randBytes.data() + halfCapacity, randBytes.data() + capacity);

}

BOOST_AUTO_TEST_CASE(testSeekPos)
{
    qint64 seekPos = capacity / 2;
    cb.write(randBytes.data(), capacity);
    cb.seek(seekPos);

    BOOST_CHECK_EQUAL(cb.pos(), seekPos);
}

BOOST_AUTO_TEST_CASE(testSeekWrite)
{
    qint64 seekPos = capacity / 2 - 1;
    qint64 bytesCount = capacity / 2;
    cb.seek(seekPos);
    cb.write(randBytes.data(), bytesCount);

    qint64 readCount = cb.read(readBytes.data(), bytesCount);

    BOOST_CHECK_EQUAL(readCount, bytesCount);
    BOOST_CHECK_EQUAL_COLLECTIONS(readBytes.data(), readBytes.data() + bytesCount,
                                  randBytes.data() + seekPos, randBytes.data() + seekPos + bytesCount);
}

BOOST_AUTO_TEST_SUITE_END()
