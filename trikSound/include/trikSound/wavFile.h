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

#pragma once

#include <vector>

#include <QFile>
#include <QAudioFormat>

#include "audioBuffer.h"
#include "trikSoundException.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT WavFile
{
public:

    static const int HEADER_SIZE = 44;

    enum OpenMode
    {
          NotOpen         = 0
        , ReadOnly        = 1
        , WriteOnly       = 2
    };

    class OpenFileException: public TrikSoundException
    {
    public:
        OpenFileException(const QString& msg):
            TrikSoundException(msg.toStdString().data())
        {}
    };

    WavFile(const QString& filename);

    /**
     * @brief open Open wav file in specified mode. If file opened for writing audio format must be specified.
     * @param mode
     * @param format
     * @return True if successfull, false otherwise.
     */
    bool open(OpenMode mode, const QAudioFormat& format = QAudioFormat());
    void close();

    QString fileName() const;
    OpenMode openMode() const;
    bool isOpen() const;
    bool isWritable() const;
    bool isReadable() const;

    bool atEnd() const;

    QAudioFormat audioFormat() const;

    /**
     * @brief size
     * @return Size of file in bytes.
     */
    qint64 size() const;
    /**
     * @brief sampleCount
     * @return Number of samples in file. If audio frame in stereo mode then it returns number of pairs of samples.
     */
    qint64 sampleCount() const;

    qint64 bytesAvailable() const;
    size_t samplesAvailable() const;

    /**
     * @brief seek Seek to the sample with specified position.
     *             If file contains stereo audio, it will actually seek to position of pair of samples (left and right).
     * @param pos
     * @return True if successfull, false otherwise.
     */
    bool seek(qint64 pos);
    /**
     * @brief pos Get current position in file.
     * @return Position of current sample.
     *         If file contains stereo audio, it will actually return position of pair of samples (left and right).
     */
    quint64 pos() const;

    /**
     * @brief read Read at most maxSize samples from file and stores it to audio buffer
     * @param maxSize
     * @return
     */
    qint64 read(char* data, qint64 size);
    AudioBuffer read(qint64 maxSize);
    AudioBuffer readAll();
    /**
     * @brief write Write audio data to the file if formats of file and buffer are equal.
     * @param buf Buffer to write
     * @return Number of samples that were actually written
     */
    qint64 write(const AudioBuffer& buf);
    qint64 write(const char* data, qint64 size);

private:

    int bytesPerSample() const;
    qint64 byteNumToSample(qint64 bytePos) const;
    qint64 sampleNumToByte(qint64 samplePos) const;

    QAudioFormat readHeader();
    bool writeHeader(const QAudioFormat& format);

    void setHeaderDataSize(quint32 size);

    QFile mFile;
    OpenMode mMode;
    QAudioFormat mAudioFormat;
};

}
