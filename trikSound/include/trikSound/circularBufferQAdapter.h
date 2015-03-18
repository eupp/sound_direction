#pragma once

#include <memory>
#include <iterator>

#include <boost/circular_buffer.hpp>

#include <QIODevice>

#include "triksound_global.h"
#include "types.h"

namespace trikSound {

/**
 * @brief The CircularBufferQAdapter class provides QIODevice interface for circular_buffer
 *        with template type equals to sample_type.
 *
 *        It is used for capturing audio data to boost circular buffer using QAudioInput.
 *
 *        CircularBufferQAdapter retains two iterators - first for writing,
 *        which is back_inserter iterator, and the second for reading from buffer.
 *        If overwriting of not read bytes occurs reading iterator is redirected to beginning of buffer.
 *
 *        CircularBufferQAdapter is not sequential device i.e. only writing to the end of buffer
 *        and forward reading are allowed.
 *
 */
class TRIKSOUNDSHARED_EXPORT CircularBufferQAdapter : public QIODevice
{
    Q_OBJECT
public:

    typedef boost::circular_buffer<sample_type> CircularBuffer;

    typedef std::shared_ptr<CircularBuffer> CircularBufferPtr;

    typedef CircularBuffer::const_iterator ReadIterator;
    typedef std::back_insert_iterator<CircularBuffer> WriteIterator;

    CircularBufferQAdapter(const CircularBufferPtr& cb, QObject *parent = 0);

    bool isSequential() const;

    virtual qint64 pos() const;
    bool seek(qint64 pos);

    bool open(OpenMode mode);
    void close();

    virtual qint64 size() const;
    bool atEnd() const;
    bool reset();

    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;

    bool canReadLine() const;

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);

    qint64 samplesAvailable() const;

    ReadIterator readBegin() const;
    ReadIterator readEnd() const;

    CircularBufferPtr buffer() const;

    void clear();

signals:

private slots:

    qint64 readData(char *data, qint64 maxlen);
    qint64 readLineData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:

    CircularBufferPtr mBuffer;
    ReadIterator mReadItr;
    WriteIterator mWriteItr;

    static const QString errorIncorrectBuffer;
};

}
