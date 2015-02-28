#include "circularBuffer.h"

#include <QDebug>

using namespace trikSound;

CircularBuffer::CircularBuffer(size_t n, QObject *parent) :
    QIODevice(parent)
  , mBuffer(new boost::circular_buffer<char>(n))
  , mPos(0)
  , mLostBytesNum(0)
  , mBytesAvailable(0)
{}

bool CircularBuffer::isSequential() const
{
    return true;
}

bool CircularBuffer::open(QIODevice::OpenMode mode)
{
    setOpenMode(mode);
    return true;
}

void CircularBuffer::close()
{
    emit aboutToClose();
    setErrorString("");
    mBuffer->clear();
    setOpenMode(NotOpen);
}

qint64 CircularBuffer::pos() const
{
    return 0;
}

qint64 CircularBuffer::size() const
{
    return mBuffer->size();
}

bool CircularBuffer::seek(qint64 pos)
{
    Q_UNUSED(pos);
    return false;
}

bool CircularBuffer::atEnd() const
{
    // circular buffer has no end
    return false;
}

bool CircularBuffer::reset()
{
    return false;
}

size_t CircularBuffer::capacity()
{
    return mBuffer->capacity();
}

void CircularBuffer::setCapacity(size_t n)
{
    mBuffer->set_capacity(n);
}

qint64 CircularBuffer::bytesAvailable() const
{
    return mBytesAvailable + QIODevice::bytesAvailable();
}

qint64 CircularBuffer::bytesToWrite() const
{
    return 0;
}

bool CircularBuffer::canReadLine() const
{
    return false;
}

bool CircularBuffer::waitForReadyRead(int msecs)
{
    Q_UNUSED(msecs);
    return true;
}

bool CircularBuffer::waitForBytesWritten(int msecs)
{
    Q_UNUSED(msecs);
    return true;
}

qint64 CircularBuffer::reserve() const
{
    return mBuffer->reserve();
}

void CircularBuffer::clear()
{
    mBuffer->clear();
    mBytesAvailable = 0;
    mPos = 0;
    QIODevice::seek(QIODevice::pos() + QIODevice::bytesAvailable());
}

qint64 CircularBuffer::readData(char* data, qint64 maxlen)
{
    qint64 len = qMin(maxlen, mBytesAvailable);
    std::uninitialized_copy(mBuffer->begin() + mPos, mBuffer->begin() + mPos + len, data);
    mPos += len;
    mBytesAvailable -= len;
    return len;
}

qint64 CircularBuffer::readLineData(char* data, qint64 maxlen)
{
    int counter = 0;
    char* ch = data;
    boost::circular_buffer<char>::iterator itr = mBuffer->begin() + mPos;
    for (; itr != mBuffer->end(), ch < data + maxlen; ++ch, ++itr) {
        if (*itr == '\n') {
            break;
        }
        *ch = *itr;
        counter++;
    }
    //mStartReadingPos = itr;
    mBytesAvailable -= counter;
    mPos += counter - 1;
    return counter;
}

qint64 CircularBuffer::writeData(const char *data, qint64 len)
{
    int reserve = 0;
    if (mBuffer->reserve() == 0) {
//		reserve = std::distance((mBuffer->begin() + mPos), mBuffer->end());
    }
    else {
        reserve = mBuffer->reserve();
    }
    if (len > reserve) {
        size_t overflow = len - reserve;
        overflow %= mBuffer->capacity();
        if (overflow == 0) {
            mPos = 0;
        }
        else {
            mPos -= overflow;
            if (mPos < 0) {
                mPos = 0;
            }
        }
    }
//	if (len > mBuffer.capacity() - bytesAvailable()) {
//		mLostBytesNum += QIODevice::bytesAvailable() - mLostBytesNum;
//	}

    // if len > size of buffer, drop all elements cached in QIODevice
    if (len > mBuffer->capacity()) {
        QIODevice::seek(QIODevice::pos() + QIODevice::bytesAvailable());
    }
    // else if len > size of non-cached part of buffer, drop only rewrited elements
    else if (len > mBuffer->capacity() - QIODevice::bytesAvailable()) {
        QIODevice::seek(QIODevice::pos() + (len - (mBuffer->capacity() - QIODevice::bytesAvailable())));
    }
    for (int i = 0; i < len; i++) {
        mBuffer->push_back(data[i]);
    }
    mBytesAvailable = qMin(mBytesAvailable + len, (qint64)(mBuffer->capacity()));
    emit readyRead();
    return qMin((qint64)mBuffer->capacity(), len);
}
