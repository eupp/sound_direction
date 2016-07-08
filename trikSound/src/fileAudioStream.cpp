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
    if (mFile.atEnd() || mFile.samplesAvailable() < mWindow.size()) {
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
