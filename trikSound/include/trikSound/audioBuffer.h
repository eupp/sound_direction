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

#include <QByteArray>
#include <QAudioFormat>

#include "triksound_global.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT AudioBuffer
{
public:

    AudioBuffer();
    AudioBuffer(const QByteArray& bytes, const QAudioFormat& format);

    QAudioFormat audioFormat() const;

    /**
     * @brief size
     * @return Size of buffer in bytes.
     */
    qint64 size() const;
    /**
     * @brief sampleCount
     * @return Number of samples in buffer. If audio frame in stereo mode then it returns number of pairs of samples.
     */
    qint64 sampleCount() const;
    /**
     * @brief duration
     * @return
     */
    qint64 duration() const;

    void* data();
    const void* data() const;
    const void* constData() const;

    AudioBuffer subBuffer(qint32 pos, qint32 len = -1);

    /**
     * @brief leftChannel
     * @return Copy bytes from left channel of stereo frame. If frame is mono it will just copy buffer.
     *         Audio format of return buffer is equalent to the format of source buffer except that it has
     *         channelsCount = 1.
     */
    AudioBuffer leftChannel() const;
    /**
     * @brief rightChannel
     * @return Copy bytes from right channel of stereo frame. If frame is mono it will just copy buffer.
     *         Audio format of return buffer is equalent to the format of source buffer except that it has
     *         channelsCount = 1.
     */
    AudioBuffer rightChannel() const;

private:
    int bytesPerSample() const;

    QByteArray mBytes;
    QAudioFormat mFormat;
};

}
