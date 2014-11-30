#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QObject>
#include <QIODevice>
#include <QSharedPointer>

#include <boost/circular_buffer.hpp>

class CircularBuffer : public QIODevice
{
    Q_OBJECT
public:
    CircularBuffer(size_t n, QObject *parent = 0);

    bool isSequential() const;

    bool open(OpenMode mode);
    void close();

    virtual qint64 pos() const;
    virtual qint64 size() const;
    bool seek(qint64 pos);
    bool atEnd() const;
    bool reset();

    size_t capacity();
    void setCapacity(size_t n);

    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;

    bool canReadLine() const;

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);

    qint64 reserve() const;

    void clear();
signals:

private slots:
    qint64 readData(char *data, qint64 maxlen);
    qint64 readLineData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    QSharedPointer< boost::circular_buffer<char> > mBuffer;
    qint64 mPos;
    qint64 mLostBytesNum;
    qint64 mBytesAvailable;
};

#endif // CIRCULARBUFFER_H
