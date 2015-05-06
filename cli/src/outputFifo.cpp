#include "outputFifo.h"

#include <cstdio>
#include <cmath>

using namespace std;

OutputFifo::OutputFifo():
    mOut(stdout, QIODevice::WriteOnly)
{}

void OutputFifo::recieve(const trikSound::AudioEvent& event)
{
    if (event.angleSetFlag()) {
        int angle = round(event.angle());
        mOut << angle << endl;
    }
}
