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

#include "thresholdVadFilter.h"

#include <algorithm>
#include <iostream>

#include <QDebug>

using namespace trikSound;

//ThresholdVadFilter::ThresholdVadFilter(double threshold):
//    mThreshold(threshold)
//{}

//double ThresholdVadFilter::threshold() const
//{
//    return mThreshold;
//}

//void ThresholdVadFilter::setThreshold(double threshold)
//{
//    mThreshold = threshold;
//}


//template <typename T>
//bool checkFrameThreshold(T threshold, const T* frame, size_t length)
//{
//    const T* pMax = std::max_element(frame, frame + length);
//    const T* pMin = std::min_element(frame, frame + length);
//    T mMax = std::abs(*pMax);
//    T mMin = std::abs(*pMin);
//    if ((mMax > threshold) || (mMin > threshold)) {
//        return true;
//    }
//    return false;
//}

//bool ThresholdVadFilter::vad(const AudioBuffer& buf)
//{
//    bool isActive = false;
//    if (buf.audioFormat().sampleSize() == 8) {
//        // hardcode -- unsigned char
//        const quint8* frame = reinterpret_cast<const quint8*>(buf.data());
//        quint8 thrld = mThreshold * std::numeric_limits<quint8>::max();
//        isActive = checkFrameThreshold(thrld, frame, buf.sampleCount());
//    }
//    else if (buf.audioFormat().sampleSize() == 16) {
//        // hardcode -- signed short
//        const qint16* frame = reinterpret_cast<const qint16*>(buf.data());
//        qint16 thrld = mThreshold * std::numeric_limits<qint16>::max();
//        isActive = checkFrameThreshold(thrld, frame, buf.sampleCount());
//    }

////    qDebug() << "Threshold Vad Filter: " << isActive;
////    std::cout << (isActive ? 1 : 0);
////    std::cout.flush();

//    return isActive;
//}

//AudioBuffer ThresholdVadFilter::input(const AudioBuffer& buf)
//{
//    bool isActive = vad(buf);

//    if (isActive) {
//        emit output(buf);
//        return buf;
//    }
//    else {
//        return AudioBuffer(QByteArray(), QAudioFormat());
//    }
//}
