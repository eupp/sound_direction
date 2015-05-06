#pragma once

#include <QTextStream>

#include "trikSound/iAudioEventListener.h"

class OutputFifo: public trikSound::IAudioEventListener
{
public:

    OutputFifo();

    void recieve(const trikSound::AudioEvent& event);

private:

    QTextStream mOut;
};
