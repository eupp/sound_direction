#pragma once

#include <QTextStream>

#include "trikSound/iAudioEventListener.h"

#include "settings.h"

class OutputFifo: public trikSound::IAudioEventListener
{
public:

    OutputFifo(const ViewSettings& settings);

    void recieve(const trikSound::AudioEvent& event);

private:

    QTextStream mOut;

    bool mShowAngle;
    bool mShowVad;
};
