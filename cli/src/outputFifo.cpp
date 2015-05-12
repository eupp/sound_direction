#include "outputFifo.h"

#include <cstdio>
#include <cmath>

using namespace std;

OutputFifo::OutputFifo(const ViewSettings& settings):
    mOut(stdout, QIODevice::WriteOnly)
  , mShowAngle(settings.showAngle())
  , mShowVad(settings.showVadCoef())
{
    mOut.setRealNumberNotation(QTextStream::FixedNotation);
    mOut.setRealNumberPrecision(8);
    mOut.setFieldWidth(4);
}

void OutputFifo::recieve(const trikSound::AudioEvent& event)
{
    static QString delim = "  ";
    if (event.vadIsActiveSetFlag() && event.vadIsActive()) {

        if (mShowAngle && event.angleSetFlag()) {
            int angle = round(event.angle());
            mOut << angle << delim;
        }
        if (mShowVad && event.vadCoefSetFlag()) {
            mOut << event.vadCoef() << delim;
        }
        mOut << endl;

    }
    else {
        mOut << "----" << endl;
    }
}
