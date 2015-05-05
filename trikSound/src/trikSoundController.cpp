#include "trikSoundController.h"

#include <vector>

#include <QDebug>
#include <QTimer>

#include "utils.h"

#include "initializer.h"

#include "audioDeviceManager.h"

#ifdef TRIK
    #include "trikAudioDeviceManager.h"
    #define AUDIO_DEVICE_MANAGER_TYPE TrikAudioDeviceManager
#else
    #define AUDIO_DEVICE_MANAGER_TYPE AudioDeviceManager
#endif

using namespace std;
using namespace boost;
using namespace trikSound;


TrikSoundController::TrikSoundController(const Settings& args,
                                         const SettingsProviderPtr& provider):
    mWindowSize(args.windowSize())
  , mWindowCopy(CHANNEL_COUNT * args.windowSize())
  , mAngleDetectionFlag(args.angleDetectionFlag())
  , mSingleChannelFlag(args.singleChannelFlag())

  , mSettingsProvider(provider)

{
    Initializer<BufferIterator> initializer(args);

    mBufferAdapter = initializer.getCircularBuffer();
    mAudioStream = initializer.getAudioStream();
    mDeviceManager = initializer.getAudioDeviceManager();
    mAngleDetector = initializer.getAngleDetector();
    mPipe = initializer.getAudioPipe();

    mAngleDetectionFlag = args.angleDetectionFlag();
    mSingleChannelFlag = args.singleChannelFlag();

    if (mSettingsProvider) {
        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateAngleDetectionHistoryDepth(int)),
                this, SLOT(setAngleDetectionHistoryDepth(int)));

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateWindowSize(size_t)),
                this, SLOT(setWindowSize(size_t)));

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateVolume(double)),
                this, SLOT(setVolume(double)));
    }

    if (args.durationFlag()) {
        QTimer::singleShot(args.duration(), this, SLOT(finish()));
    }
}

void TrikSoundController::addAudioEventListener(const TrikSoundController::ListenerPtr& listener)
{
    mListeners.push_back(listener);
}

void TrikSoundController::bufferReadyReadHandler()
{
    while (mAudioStream->samplesAvailable() >= mWindowSize) {

        if (mSingleChannelFlag) {
            handleSingleChannel();
        }
        else {
            handleDoubleChannel();
        }

        AudioEvent event;
        if (mAngleDetectionFlag) {
            assert(mAngleDetector != nullptr);
            event.setAngle(mAngleDetector->getAngle());
        }

        notify(event);
    }

}

void TrikSoundController::handleSingleChannel()
{
    mAudioStream->read(mWindowCopy.data());
    auto begin = mWindowCopy.begin();
    auto end   = begin + mWindowSize;

    mPipe->handleWindow(make_pair(begin, end),
                        StereoAudioFilter<BufferIterator>::make_empty_range());
}

void TrikSoundController::handleDoubleChannel()
{
    mAudioStream->read(mWindowCopy.data());
    auto leftBegin  = mWindowCopy.begin();
    auto leftEnd    = leftBegin + mWindowSize;
    auto rightBegin = leftEnd;
    auto rightEnd   = mWindowCopy.end();

    mPipe->handleWindow(make_pair(leftBegin , leftEnd),
                        make_pair(rightBegin, rightEnd));
}

void TrikSoundController::notify(const AudioEvent& event)
{
    for (auto& listener: mListeners) {
        listener->recieve(event);
    }
}

void TrikSoundController::run()
{
    connect(mAudioStream.get(), SIGNAL(readyRead()), this, SLOT(bufferReadyReadHandler()));
    mAudioStream->run();
}

void TrikSoundController::restart()
{
    stop();
    mBufferAdapter->clear();
    // in case of changes in windowSize
    mBufferAdapter->resize(Initializer<BufferIterator>::BUFFER_CAPACITY * mWindowSize);
    run();
}

void TrikSoundController::stop()
{
    mAudioStream->stop();
    disconnect(mAudioStream.get(), 0, this, 0);
}

void TrikSoundController::finish()
{
    stop();
    emit finished();
}

bool TrikSoundController::singleChannelFlag() const
{
    return mSingleChannelFlag;
}

int TrikSoundController::angleDetectionHistoryDepth() const
{
    return mAngleDetector ? mAngleDetector->historyDepth() : 0;
}

size_t TrikSoundController::windowSize() const
{
    return mWindowSize;
}

double TrikSoundController::volume() const
{
    return mDeviceManager->volume();
}

void TrikSoundController::setAngleDetectionHistoryDepth(int historyDepth)
{
    if (mAngleDetector) {
        mAngleDetector->setHistoryDepth(historyDepth);
        restart();
    }
}

void TrikSoundController::setWindowSize(size_t size)
{
    mWindowSize = size;
    restart();
}

void TrikSoundController::setVolume(double vol)
{
    mDeviceManager->setVolume(vol);
    restart();
}

