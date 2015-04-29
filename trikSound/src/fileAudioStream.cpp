#include "fileAudioStream.h"

namespace trikSound
{

FileAudioStream::FileAudioStream(const QString& filename, QObject* parent):
    AudioStream(parent)
  , mFile(filename)
{
    mFile.open(WavFile::ReadOnly);
}

void FileAudioStream::run()
{
    emit readyRead();
}

void FileAudioStream::stop()
{

}

void FileAudioStream::read(sample_type* buf, size_t size)
{
    mFile.read((char*)buf, size * sizeof(sample_type));
}

size_t FileAudioStream::samplesAvailable() const
{
    return mFile.samplesAvailable() / mFile.audioFormat().channelCount();
}



}
