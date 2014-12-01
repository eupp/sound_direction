#include "include/internal/angleFilter.h"

#include <iostream>

using namespace std;

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

        cout << "Angle: " << angle << endl;
    }

    return AudioBuffer(QByteArray(), QAudioFormat());
}


