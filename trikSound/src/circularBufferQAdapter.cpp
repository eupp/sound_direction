/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
{}

CircularBufferQAdapter::CircularBufferPtr CircularBufferQAdapter::getCircularBuffer() const
{
    return mBuffer;
}

void CircularBufferQAdapter::setCircularBuffer(const CircularBufferQAdapter::CircularBufferPtr& cb)
{
    mBuffer = cb;
    setOpenMode(QIODevice::NotOpen);
}

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
    return mBuffer->size    () * sizeof(sample_type);
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

void CircularBufferQAdapter::resize(size_t size)
{
    mBuffer->resize(size);
}

qint64 CircularBufferQAdapter::samplesAvailable() const
{
    return mBuffer->samplesAvailable();
}

void CircularBufferQAdapter::clear()
{
    mBuffer->clear();
}

int CircularBufferQAdapter::channelCount() const
{
    return mBuffer->channelCount();
}

qint64 CircularBufferQAdapter::readData(char* data, qint64 maxlen)
{
    // check that buffer and len are fit to read sample data
    if (maxlen % sizeof(sample_type) != 0) {
        setErrorString(errorIncorrectBuffer);
        qDebug() << errorString();
        return -1;
    }

    qint64 sampleCount = min( maxlen / (qint64)sizeof(sample_type), samplesAvailable());
    sample_type* sampleData = reinterpret_cast<sample_type*>(data);

    if (sampleCount % mBuffer->channelCount() != 0) {
        setErrorString(errorIncorrectBuffer);
        qDebug() << errorString();
        return -1;
    }

    mBuffer->read(sampleData, sampleCount);

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

    if (sampleCount % mBuffer->channelCount() != 0) {
        setErrorString(errorIncorrectBuffer);
        qDebug() << errorString();
        return -1;
    }

    mBuffer->write(sampleData, sampleCount);

    emit readyRead();
    return sampleCount * sizeof(sample_type);
}
