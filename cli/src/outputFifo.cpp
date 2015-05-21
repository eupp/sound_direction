#include "outputFifo.h"

#include <trikControl/brickFactory.h>
#include <trikControl/brickInterface.h>

#include <cstdio>
#include <cmath>
#include <algorithm>


using namespace std;
using namespace trikControl;


OutputFifo::OutputFifo(const ViewSettings& settings):
    mOut(stdout, QIODevice::WriteOnly)
  , mShowAngle(settings.showAngle())
  , mShowVad(settings.showVadCoef())
  , mDiffTime(settings.diffTime())

  , mEnrg(0)
{
    mOut.setRealNumberNotation(QTextStream::FixedNotation);
    mOut.setRealNumberPrecision(12);
    mOut.setFieldWidth(4);

    mAngles.reserve(100);

    BrickInterface* brick = BrickFactory::create("../trik/", "./");
    mMotor = brick->motor("C1");
    mMotor->setPower(0);
}

void OutputFifo::recieve(const trikSound::AudioEvent& event)
{
    if (abs(event.timestamp() - mPrevTimestamp) / TO_MS_COEFF > mDiffTime) {
        printEventData(event);
    }
    if (event.vadIsActiveSetFlag() && event.vadIsActive()) {
        saveEventData(event);
    }
    else {
//        mOut << "----" << endl;
    }
}

void OutputFifo::printEventData(const trikSound::AudioEvent& event)
{
    if (mAngles.empty()) {
        return;
    }

    static QString delim = "  ";

    int med = mAngles.size() / 2;
    nth_element(mAngles.begin(), mAngles.begin() + med, mAngles.end());
    int angle = mAngles[med];
    double thrsd = mEnrg / mAngles.size();

//    int angle = accumulate(mAngles.begin(), mAngles.end(), 0);
//    angle /= (int)mAngles.size();

    if (mShowAngle) {
//        mOut << "Angle calculated: " << angle << delim;
        mOut << angle << delim;

        mMotor->setPower(angle);
    }
    if (mShowVad) {
        mOut << thrsd << delim;
    }
    mOut << endl;

    mAngles.clear();
    mEnrg = 0;
}

void OutputFifo::saveEventData(const trikSound::AudioEvent& event)
{
    if (mShowAngle && event.angleSetFlag()) {
        int angle = round(event.angle());
        mAngles.push_back(angle);

        mOut << "Angle received: " <<  angle << endl;
    }
    if (mShowVad && event.vadCoefSetFlag()) {
         mEnrg += event.vadCoef();
    }
    mPrevTimestamp = event.timestamp();
}
