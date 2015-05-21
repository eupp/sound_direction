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
                                         const SettingsProviderPtr& provider,
                                         QObject* parent):
    QObject(parent)

  , mWindowSize(args.windowSize())
  , mWindowCopy(CHANNEL_COUNT * args.windowSize())

  , mVadFlag(args.vadFlag())
  , mAngleDetectionFlag(args.angleDetectionFlag())
  , mSingleChannelFlag(args.singleChannelFlag())

  , mSettingsProvider(provider)

  , mTimeoutFlag(args.durationFlag())
  , mTimeout(1000 * args.duration())

{
    Initializer<BufferIterator> initializer(args);

    mBufferAdapter = initializer.getCircularBuffer();
    mAudioStream = initializer.getAudioStream();
    mDeviceManager = initializer.getAudioDeviceManager();
    mAngleDetector = initializer.getAngleDetector();
    mVad = initializer.getVadWrapper();
    mPipe = initializer.getAudioPipe();

    connect(mAudioStream.get(), SIGNAL(finished()), this, SLOT(finish()), Qt::QueuedConnection);

    if (mSettingsProvider) {
        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateAngleDetectionHistoryDepth(int)),
                this, SLOT(setAngleDetectionHistoryDepth(int)), Qt::QueuedConnection);

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateWindowSize(quint64)),
                this, SLOT(setWindowSize(quint64)), Qt::QueuedConnection);

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateVolume(double)),
                this, SLOT(setVolume(double)), Qt::QueuedConnection);
    }
}

void TrikSoundController::addAudioEventListener(const TrikSoundController::ListenerPtr& listener)
{
    mListeners.push_back(listener);
}

void TrikSoundController::bufferReadyReadHandler()
{
    while (mAudioStream->samplesAvailable() >= mWindowSize) {


        // get opportunity to waiting events to be processed
        // before handle next window
//        processEvents();

        if (mSingleChannelFlag) {
            handleSingleChannel();
        }
        else {
            handleDoubleChannel();
        }

        AudioEvent event;
        if (mAngleDetectionFlag) {
            assert(mAngleDetector != nullptr);
            if (mAngleDetector->updated()) {
                event.setAngle(mAngleDetector->getAngle());
            }
        }
        if (mVadFlag) {
            assert(mVad != nullptr);
            event.setVadCoef(mVad->getEnergyCoefficient());
            event.setVadIsActive(mVad->isActive());
        }

        notify(event);
    }
}

void TrikSoundController::processEvents()
{
    mSettingsProvider->processEvents();
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
    connect(mAudioStream.get(), SIGNAL(readyRead()), this, SLOT(bufferReadyReadHandler()),
            Qt::DirectConnection);
    if (mTimeoutFlag) {
        QTimer::singleShot(mTimeout, this, SLOT(finish()));
    }
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

void TrikSoundController::setWindowSize(quint64 size)
{
    mWindowSize = size;
    restart();
}

void TrikSoundController::setVolume(double vol)
{
    mDeviceManager->setVolume(vol);
    restart();
}

