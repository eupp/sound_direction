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

#include <ctime>
#include <vector>

#include <QTextStream>

#include "trikSound/iAudioEventListener.h"

#include "settings.h"

class OutputFifo: public trikSound::IAudioEventListener
{
public:

    OutputFifo(const ViewSettings& settings);

    void recieve(const trikSound::AudioEvent& event);

private:

    void printEventData(const trikSound::AudioEvent& event);
    void saveEventData(const trikSound::AudioEvent& event);

    const int TO_MS_COEFF = CLOCKS_PER_SEC / 1000;

    QTextStream mOut;

    bool mShowAngle;
    bool mShowVad;

    clock_t mPrevTimestamp;
    bool mEventFlag;
    std::vector<int> mAngles;
    int mFrameCnt;
    double mEnrg;

    int mDiffTime;
};
