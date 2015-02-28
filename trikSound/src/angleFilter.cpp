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


