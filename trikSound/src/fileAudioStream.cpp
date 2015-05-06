#include "fileAudioStream.h"

#include <cassert>

#include "utils.h"

namespace trikSound
{

FileAudioStream::FileAudioStream(const QString& filename, size_t windowSize, QObject* parent):
    AudioStream(parent)
  , mFile(filename)
  , mWindow(windowSize)
{
    assert(windowSize % 2 == 0);

    mFile.open(WavFile::ReadOnly);
}

void FileAudioStream::run()
{
    emit readyRead();
}

void FileAudioStream::stop()
{

}

void FileAudioStream::read(sample_type* buf)
{
    if (mFile.audioFormat().channelCount() == 2) {
        mFile.read((char*)mWindow.data(), mWindow.size() * sizeof(sample_type));
        extractChannel<2, 0>(mWindow.begin(), mWindow.end(), buf);
        extractChannel<2, 1>(mWindow.begin(), mWindow.end(), buf + windowSize() / 2);
    }
    else {
        mFile.read((char*)buf, mWindow.size() * sizeof(sample_type));
    }
    if (mFile.atEnd()) {
        emit finished();
    }
}

size_t FileAudioStream::samplesAvailable() const
{
    return mFile.samplesAvailable() / mFile.audioFormat().channelCount();
}

size_t FileAudioStream::windowSize() const
{
    return mWindow.size();
}

void FileAudioStream::setWindowSize(size_t size)
{
    mWindow.resize(size);
}



}
