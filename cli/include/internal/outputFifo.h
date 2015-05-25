#pragma once

#include <ctime>
#include <vector>

#include <QTextStream>

#include <trikControl/motorInterface.h>

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
    double mEnrg;

    int mDiffTime;

    trikControl::MotorInterface* mMotor;
};
