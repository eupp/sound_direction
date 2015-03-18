#include "trikSoundController.h"

#include <vector>

#include "utils.h"

#include "emptyFilter.h"
#include "emptyStereoFilter.h"
#include "digitalAudioFilter.h"
#include "splitFilter.h"

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


TrikSoundController::TrikSoundController(const TrikSoundController::Settings& args,
                                         const SettingsProviderPtr& provider):

    mBuffer(make_shared<CircularBufferQAdapter::CircularBuffer>(BUFFER_CAPACITY * args.windowSize()))
  , mBufferAdapter(make_shared<CircularBufferQAdapter>(mBuffer))
  , mWindowSize(args.windowSize())
  , mWindowCopy(2 * args.windowSize())
  , mFilter(make_shared<EmptyFilter<BufferIterator>>())
  , mStereoFilter(make_shared<EmptyStereoFilter<BufferIterator>>())
  , mAngleDetectionFlag(args.angleDetectionFlag())
  , mSettingsProvider(provider)

{
    QAudioDeviceInfo dev = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat fmt;
    fmt.setSampleRate(args.sampleRate());
    fmt.setSampleSize(args.sampleSize());
    fmt.setSampleType(args.sampleType());
    if (!fmt.isValid()) {
        throw InitException("TrikSoundController error. Invalid audio format");
    }
    mDeviceManager.reset(new AUDIO_DEVICE_MANAGER_TYPE(dev, fmt, mBufferAdapter));

    if (args.filteringFlag()) {
        FilterPtr filter = make_shared<DigitalAudioFilter<BufferIterator>>();
        mFilter->insertFilter(filter);
    }

    if (!args.singleChannelFlag()) {
        StereoFilterPtr split = make_shared<SplitFilter<BufferIterator>>(mFilter);
        mStereoFilter->insertFilter(split);
    }
    if (args.angleDetectionFlag()) {
        if (args.singleChannelFlag()) {
            throw InitException("TrikSoundController error. Angle detection enabled with single audio channel");
        }

        mAngleDetector = make_shared<AngleDetector<BufferIterator>>(args.sampleRate(),
                                                                   args.micrDist(),
                                                                   args.angleDetectionHistoryDepth());
        StereoFilterPtr detector = mAngleDetector;
        mStereoFilter->insertFilter(detector);
    }

    connect(mSettingsProvider.get(), SIGNAL(updateAngleDetectionHistoryDepth(int)),
            this, SLOT(setAngleDetectionHistoryDepth(int)));
    connect(mSettingsProvider.get(), SIGNAL(updateWindowSize(size_t)), this, SLOT(setWindowSize(size_t)));
    connect(mSettingsProvider.get(), SIGNAL(updateVolume(double)), this, SLOT(setVolume(double)));
}

void TrikSoundController::addAudioEventListener(const TrikSoundController::ListenerPtr& listener)
{
    mListeners.push_back(listener);
}

void TrikSoundController::bufferReadyReadHandler()
{
    if (mBufferAdapter->samplesAvailable() < CHANNEL_COUNT * mWindowSize) {
        return;
    }

    if (mSingleChannelFlag) {
        handleSingleChannel();
    }
    else {
        handleDoubleChannel();
    }

    AudioEvent event;
    if (mAngleDetectionFlag) {
        event.setAngle(mAngleDetector->getAngle());
    }

    notify(event);
}

void TrikSoundController::handleSingleChannel()
{
    auto chlBegin = mWindowCopy.begin();
    auto chlEnd   = chlBegin + mWindowSize;
    copy(mBufferAdapter->readBegin(), mBufferAdapter->readEnd(), chlBegin);
    mFilter->handleWindow(chlBegin, chlEnd);
}

void TrikSoundController::handleDoubleChannel()
{
    auto chl1Begin = mWindowCopy.begin();
    auto chl1End   = chl1Begin + mWindowSize;
    auto chl2Begin = chl1End;
    auto chl2End   = mWindowCopy.end();

    extractChannel<CHANNEL_COUNT, 0>(mBufferAdapter->readBegin(), mBufferAdapter->readEnd(), chl1Begin);
    extractChannel<CHANNEL_COUNT, 1>(mBufferAdapter->readBegin(), mBufferAdapter->readEnd(), chl2Begin);

    mStereoFilter->handleWindow(make_pair(chl1Begin, chl1End),
                                make_pair(chl2Begin, chl2End));
}

void TrikSoundController::notify(const AudioEvent& event)
{
    for (auto& listener: mListeners) {
        listener->recieve(event);
    }
}

bool TrikSoundController::singleChannelFlag() const
{
    return mSingleChannelFlag;
}

void TrikSoundController::run()
{
    connect(mBufferAdapter.get(), SIGNAL(readyRead()), this, SLOT(bufferReadyReadHandler()));
    mDeviceManager->start();
}

void TrikSoundController::restart()
{
    stop();
    run();
}

void TrikSoundController::stop()
{
    mDeviceManager->stop();
}

int TrikSoundController::angleDetectionHistoryDepth() const
{
    return mAngleDetector->historyDepth();
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
    mAngleDetector->setHistoryDepth(historyDepth);
    restart();
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


int TrikSoundController::Settings::angleDetectionHistoryDepth() const
{
    return mAngleDetectionHistoryDepth;
}

void TrikSoundController::Settings::setAngleDetectionHistoryDepth(int angleDetectionHistoryDepth)
{
    mAngleDetectionHistoryDepth = angleDetectionHistoryDepth;
}

size_t TrikSoundController::Settings::windowSize() const
{
    return mWindowSize;
}

void TrikSoundController::Settings::setWindowSize(const size_t& windowSize)
{
    mWindowSize = windowSize;
}

double TrikSoundController::Settings::volume() const
{
    return mVolume;
}

void TrikSoundController::Settings::setVolume(double volume)
{
    mVolume = volume;
}

QAudioFormat::SampleType TrikSoundController::Settings::sampleType() const
{
    return mSampleType;
}

void TrikSoundController::Settings::setSampleType(const QAudioFormat::SampleType& sampleType)
{
    mSampleType = sampleType;
}

int TrikSoundController::Settings::sampleSize() const
{
    return mSampleSize;
}

void TrikSoundController::Settings::setSampleSize(int sampleSize)
{
    mSampleSize = sampleSize;
}

int TrikSoundController::Settings::sampleRate() const
{
    return mSampleRate;
}

void TrikSoundController::Settings::setSampleRate(int sampleRate)
{
    mSampleRate = sampleRate;
}

bool TrikSoundController::Settings::angleDetectionFlag() const
{
    return mAngleDetectionFlag;
}

void TrikSoundController::Settings::setAngleDetectionFlag(bool angleDetectionFlag)
{
    mAngleDetectionFlag = angleDetectionFlag;
}

bool TrikSoundController::Settings::filteringFlag() const
{
    return mFilteringFlag;
}

void TrikSoundController::Settings::setFilteringFlag(bool filteringFlag)
{
    mFilteringFlag = filteringFlag;
}

bool TrikSoundController::Settings::singleChannelFlag() const
{
    return mSingleChannelFlag;
}

void TrikSoundController::Settings::setSingleChannelFlag(bool singleChannelFlag)
{
    mSingleChannelFlag = singleChannelFlag;
}

double TrikSoundController::Settings::micrDist() const
{
    return mMicrDist;
}

void TrikSoundController::Settings::setMicrDist(double micrDist)
{
    mMicrDist = micrDist;
}
