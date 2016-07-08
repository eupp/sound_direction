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

#include "angleFilter.h"

#include <iostream>

using namespace std;
using namespace trikSound;

AngleFilter::AngleFilter(double micrDist, double threshold):
    mFilter(new DigitalAudioFilter())
  , mVad(threshold)
  , mMicrDist(micrDist)
{}

AudioBuffer AngleFilter::input(const AudioBuffer& buf)
{
    AudioBuffer leftCh = buf.leftChannel();
    AudioBuffer rightCh = buf.rightChannel();
    AudioBuffer filt1 = mFilter->input(leftCh);
    AudioBuffer filt2 = mFilter->input(rightCh);
    bool isActive = mVad.vad(filt1) || mVad.vad(filt2);
    if (isActive) {
        double angle = mDetector.getAngle(filt1, filt2, mMicrDist);

        std::cout.setf( std::ios::fixed, std:: ios::floatfield );
        std::cout.precision(2);
        cout << "Angle: " << angle << endl;
    }

    return AudioBuffer(QByteArray(), QAudioFormat());
}


