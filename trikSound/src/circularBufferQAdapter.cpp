#include "circularBufferQAdapter.h"

#include <algorithm>

#include <QDebug>

using namespace std;
using namespace trikSound;

const QString CircularBufferQAdapter::errorIncorrectBuffer =
        "CircularBufferQAdapter Error: Incorrect buffer passed";

CircularBufferQAdapter::CircularBufferQAdapter(const CircularBufferQAdapter::CircularBufferPtr& cb,
                                               QObject* parent):
    QIODevice(parent)
  , mBuffer(cb)
  , mReadItr(cb->begin())
  , mWriteItr(back_inserter(*cb))
{}

bool CircularBufferQAdapter::isSequential() const
{
    return true;
}

qint64 CircularBufferQAdapter::pos() const
{
    return 0;
}

bool CircularBufferQAdapter::seek(qint64 pos)
{
    Q_UNUSED(pos);
    return false;
}

bool CircularBufferQAdapter::open(QIODevice::OpenMode mode)
{
    return QIODevice::open(mode | QIODevice::Unbuffered);
}

void CircularBufferQAdapter::close()
{
    QIODevice::close();
}

qint64 CircularBufferQAdapter::size() const
{
    return mBuffer->size() * sizeof(sample_type);
}


bool CircularBufferQAdapter::atEnd() const
{
    // circular buffer has no end
    return false;
}

bool CircularBufferQAdapter::reset()
{
    return false;
}

qint64 CircularBufferQAdapter::bytesAvailable() const
{
    return samplesAvailable() * sizeof(sample_type);
}

qint64 CircularBufferQAdapter::bytesToWrite() const
{
    return 0;
}

bool CircularBufferQAdapter::canReadLine() const
{
    return false;
}

bool CircularBufferQAdapter::waitForReadyRead(int msecs)
{
    Q_UNUSED(msecs);
    return false;
}

bool CircularBufferQAdapter::waitForBytesWritten(int msecs)
{
    Q_UNUSED(msecs);
    return false;
}

qint64 CircularBufferQAdapter::samplesAvailable() const
{
    return (mBuffer->end() - mReadItr);
}

CircularBufferQAdapter::ReadIterator CircularBufferQAdapter::readBegin() const
{
    return mReadItr;
}

CircularBufferQAdapter::ReadIterator CircularBufferQAdapter::readEnd() const
{
    return mBuffer->end();
}

CircularBufferQAdapter::CircularBufferPtr CircularBufferQAdapter::buffer() const
{
    return mBuffer;
}

void CircularBufferQAdapter::clear()
{
    mBuffer->clear();
    mReadItr = mBuffer->begin();
}

qint64 CircularBufferQAdapter::readData(char* data, qint64 maxlen)
{
    // check that buffer and len are fit to read sample data
    if (maxlen % sizeof(sample_type) != 0) {
        setErrorString(errorIncorrectBuffer);
        qDebug() << errorString();
        return -1;
    }

    qint64 sampleCount = min(maxlen, bytesAvailable()) / sizeof(sample_type);
    sample_type* sampleData = reinterpret_cast<sample_type*>(data);

    if (mReadItr == mBuffer->end()) {
        qDebug() << "halt";
    }

    auto readEnd = mReadItr + sampleCount;
    copy(mReadItr, readEnd, sampleData);
    mReadItr = readEnd;

    return sampleCount * sizeof(sample_type);
}

qint64 CircularBufferQAdapter::readLineData(char* data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return -1;
}

qint64 CircularBufferQAdapter::writeData(const char *data, qint64 len)
{
    // check that correct audio data is written to buffer
    if (len % sizeof(sample_type) != 0) {
        setErrorString(errorIncorrectBuffer);
        qDebug() << errorString();
        return -1;
    }

    qint64 sampleCount = len / sizeof(sample_type);
    const sample_type* sampleData = reinterpret_cast<const sample_type*>(data);

    bool overwriteFlag = false;
    int freeSpace = (mReadItr - mBuffer->begin()) + (mBuffer->capacity() - mBuffer->size());
    if (sampleCount > freeSpace) {
        overwriteFlag = true;
    }

    // special case for writing to the empty container
    // in that case mReadItr == cb.begin() == cb.end()
    bool emptyFlag = mBuffer->empty();

    copy(sampleData, sampleData + sampleCount, mWriteItr);

    // redirect read iterator to begin in case of overwriting or empty buffer
    if (overwriteFlag || emptyFlag) {
        mReadItr = mBuffer->begin();
    }

    return sampleCount * sizeof(sample_type);
}
