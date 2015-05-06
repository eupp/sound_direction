#include "wavFile.h"

#include <cassert>
#include <cstring>
#include <exception>
#include <limits>

#include <QDebug>

#include "types.h"

using namespace trikSound;

class UncorrectHeaderExc: public std::exception
{};

struct WavHeader
{
    quint8    chunkId[4];
    quint32   chunkSize;
    quint8    format[4];
    quint8    subchunk1Id[4];
    quint32   subchunk1Size;
    quint16   audioFormat;
    quint16   numChannels;
    quint32   sampleRate;
    quint32   byteRate;
    quint16   blockAlign;
    quint16   bitsPerSample;
    quint8    subchunk2Id[4];
    quint32   subchunk2Size;
};

WavFile::WavFile(const QString& filename):
    mFile(filename)
  , mMode(NotOpen)
{}

bool WavFile::open(WavFile::OpenMode mode, const QAudioFormat& format)
{
    close();
    if (mode == ReadOnly) {
        if (!mFile.open(QIODevice::ReadOnly)) {
            throw OpenFileException("Cannot open wav file: " + fileName() + "; error: " + mFile.errorString());
        }
        try {
            mAudioFormat = readHeader();
        }
        catch (UncorrectHeaderExc& exc) {
            mFile.close();
            throw OpenFileException("Cannot open wav file: " + fileName() + "; error: Incorrect header");
        }
    }
    else if (mode == WriteOnly) {
        if (!format.isValid()) {
            throw OpenFileException("Cannot open wav file: " + fileName() + "; error: Incorrect audio format");
        }
        if (!mFile.open(QIODevice::WriteOnly)) {
            throw OpenFileException("Cannot open wav file: " + fileName() + "; error: " + mFile.errorString());
        }
        if (!writeHeader(format)) {
            mFile.close();
            throw OpenFileException("Cannot open wav file: " + fileName() + "; error: Cannot write wav header");
        }
        mAudioFormat = format;
    }
    mFile.seek(HEADER_SIZE);
    mMode = mode;
    return true;
}

void WavFile::close()
{
    mFile.close();
    mMode = NotOpen;
}

QString WavFile::fileName() const
{
    return mFile.fileName();
}

WavFile::OpenMode WavFile::openMode() const
{
    return mMode;
}

bool WavFile::isOpen() const
{
    return mMode != NotOpen;
}

bool WavFile::isWritable() const
{
    return mMode == WriteOnly;
}

bool WavFile::isReadable() const
{
    return mMode == ReadOnly;
}

bool WavFile::atEnd() const
{
    return mFile.atEnd();
}

QAudioFormat WavFile::audioFormat() const
{
    return mAudioFormat;
}

qint64 WavFile::size() const
{
    return mFile.size();
}

qint64 WavFile::sampleCount() const
{
    qint64 count = (mFile.size() - HEADER_SIZE) / bytesPerSample();
    if (mAudioFormat.channelCount() == 2) {
        count /= 2;
    }
    return count;
}

qint64 WavFile::bytesAvailable() const
{
    return mFile.bytesAvailable();
}

size_t WavFile::samplesAvailable() const
{
    return mFile.bytesAvailable() / sizeof(sample_type);
}

bool WavFile::seek(qint64 pos)
{
    return mFile.seek(HEADER_SIZE + sampleNumToByte(pos));
}

quint64 WavFile::pos() const
{
    return byteNumToSample(mFile.pos() - HEADER_SIZE);
}

qint64 WavFile::read(char* data, qint64 size)
{
    return mFile.read(data, size);
}

AudioBuffer WavFile::read(qint64 maxSize)
{
    return AudioBuffer(mFile.read(sampleNumToByte(maxSize)), mAudioFormat);
}

AudioBuffer WavFile::readAll()
{
    return AudioBuffer(mFile.readAll(), mAudioFormat);
}

qint64 WavFile::write(const AudioBuffer& buf)
{
    return write((char*)buf.data(), buf.size());
}

qint64 WavFile::write(const char* data, qint64 size)
{
    if (size % bytesPerSample() != 0) {
        return -1;
    }
    qint64 bytesForWriting = qMin(size, std::numeric_limits<quint32>::max() - mFile.size());
    qint64 bytesWritten    = mFile.write(data, bytesForWriting);
    setHeaderDataSize(mFile.size());
    return bytesWritten;
}

int WavFile::bytesPerSample() const
{
    int fs = mAudioFormat.sampleRate();
    int tmp = mAudioFormat.sampleSize();
    return mAudioFormat.sampleSize() / 8;
}

qint64 WavFile::byteNumToSample(qint64 bytePos) const
{
    return bytePos / (bytesPerSample() * mAudioFormat.channelCount());
}

qint64 WavFile::sampleNumToByte(qint64 samplePos) const
{
    return samplePos * bytesPerSample() * mAudioFormat.channelCount();
}


QAudioFormat WavFile::readHeader()
{
    assert(sizeof(WavHeader) == HEADER_SIZE);

    if (mFile.size() < sizeof(WavHeader)) {
        throw UncorrectHeaderExc();
    }

    qint64 currPos = mFile.pos();
    mFile.seek(0);
    WavHeader header;
    memset((void*) &header, 0, sizeof(WavHeader));
    mFile.read((char*) &header, sizeof(WavHeader));
    mFile.seek(currPos);

    // check that header is correct wav header
    if (strncmp((char*) header.chunkId, "RIFF", 4) != 0) {
        throw UncorrectHeaderExc();
    }

    if (strncmp((char*) header.format, "WAVE", 4) != 0) {
        throw UncorrectHeaderExc();
    }

    if (strncmp((char*) header.subchunk1Id, "fmt ", 4) != 0) {
        throw UncorrectHeaderExc();
    }

    if (strncmp((char*) header.subchunk2Id, "data", 4) != 0) {
        throw UncorrectHeaderExc();
    }

    QAudioFormat format;
    // pcm format hardcoded
    format.setCodec("audio/pcm");
    format.setSampleRate(header.sampleRate);
    format.setChannelCount (header.numChannels);
    format.setSampleSize(header.bitsPerSample);
    if (header.bitsPerSample == 8) {
        format.setSampleType(QAudioFormat::UnSignedInt);
    }
    else if (header.bitsPerSample == 16) {
        format.setSampleType(QAudioFormat::SignedInt);
    }
    // LE hardcoded
    format.setByteOrder(QAudioFormat::LittleEndian);

    return format;
}

bool WavFile::writeHeader(const QAudioFormat& format)
{
    assert(sizeof(WavHeader) == HEADER_SIZE);

    WavHeader header;
    memset((void*) &header, 0, sizeof(WavHeader));

    strncpy((char*)header.chunkId, "RIFF", 4);
    strncpy((char*)header.format,  "WAVE", 4);
    strncpy((char*)header.subchunk1Id, "fmt ", 4);
    strncpy((char*)header.subchunk2Id, "data", 4);

    header.chunkSize = 36;
    header.subchunk1Size = 16;
    // PCM format == 1
    header.audioFormat = 1;
    header.numChannels = format.channelCount();
    header.sampleRate = format.sampleRate();
    header.byteRate = format.sampleRate() * format.channelCount() * (format.sampleSize() / 8);
    header.blockAlign = format.channelCount() * (format.sampleSize() / 8);
    header.bitsPerSample = format.sampleSize();

    return mFile.write((char*) &header, sizeof(WavHeader)) == sizeof(WavHeader);
}

void WavFile::setHeaderDataSize(quint32 size)
{
//    qDebug() << "Wav header: data size = " << size;

    qint64 currPos = mFile.pos();
    mFile.seek(HEADER_SIZE - 4);
    mFile.write(reinterpret_cast<char*>(&size), sizeof(qint32));
    mFile.seek(currPos);
}
