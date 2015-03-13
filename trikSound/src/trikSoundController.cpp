#include "trikSoundController.h"

#include <vector>

#include "utils.h"

using namespace std;
using namespace boost;
using namespace trikSound;


void TrikSoundController::setAngleDetectionHistoryDepth(int historyDepth)
{

}

void TrikSoundController::setFrameLength(size_t length)
{

}

void TrikSoundController::setVolume(double vol)
{

}

void TrikSoundController::bufferReadyReadHandler()
{
    if (mBufferAdapter.samplesAvailable() < CHANNEL_COUNT * mFrameLength) {
        return;
    }

    auto chl1Begin = mWindowCopy.begin();
    auto chl1End   = chl1Begin + mFrameLength;
    auto chl2Begin = chl1End;
    auto chl2End   = mWindowCopy.end();

    extractChannel<CHANNEL_COUNT, 0>(mBufferAdapter.readBegin(), mBufferAdapter.readEnd(), chl1Begin);
    extractChannel<CHANNEL_COUNT, 1>(mBufferAdapter.readBegin(), mBufferAdapter.readEnd(), chl2Begin);

    mFilter->handleWindow(make_pair(chl1Begin, chl1End),
                          make_pair(chl2Begin, chl2End));


    AudioEvent event;
    if (mAngleDetectionFlag) {
        event.setAngle(mAngleDetector->getAngle());
    }

    notify(event);
}

void TrikSoundController::notify(const AudioEvent& event)
{
    for (auto& listener: mListeners) {
        listener->recieve(event);
    }
}
