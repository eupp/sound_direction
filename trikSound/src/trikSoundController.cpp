#include "trikSoundController.h"

#include <vector>

#include <QDebug>
#include <QTimer>

#include "utils.h"

#include "emptyFilter.h"
#include "emptyStereoFilter.h"
#include "digitalAudioFilter.h"
#include "splitFilter.h"
#include "recordFilter.h"
#include "stereoRecordFilter.h"

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
  , mTmpWindowCopy(2 * args.windowSize())
  , mFilter(make_shared<EmptyFilter<BufferIterator>>())
  , mStereoFilter(make_shared<EmptyStereoFilter<BufferIterator>>())
  , mAngleDetectionFlag(args.angleDetectionFlag())
  , mSingleChannelFlag(args.singleChannelFlag())

  , mSettingsProvider(provider)

{
    mBufferAdapter->open(QIODevice::ReadWrite);

    QAudioDeviceInfo dev = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat fmt;
    fmt.setSampleRate(args.sampleRate());
    fmt.setSampleSize(args.sampleSize());
    fmt.setSampleType(args.sampleType());
    fmt.setCodec("audio/pcm");
    if (args.singleChannelFlag()) {
        fmt.setChannelCount(1);
    }
    else {
        fmt.setChannelCount(2);
    }
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
    if (args.recordStreamFlag()) {
        RecordFilter<BufferIterator>::WavFilePtr wavFile = make_shared<WavFile>(args.outputWavFilename());
        wavFile->open(WavFile::WriteOnly, fmt);
        if (args.singleChannelFlag()) {
            FilterPtr record = make_shared<RecordFilter<BufferIterator>>(wavFile);
            mFilter->insertFilter(record);
        }
        else {
            StereoFilterPtr record = make_shared<StereoRecordFilter<BufferIterator>>(wavFile);
            mStereoFilter->insertFilter(record);
        }
    }

    if (mSettingsProvider) {
        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateAngleDetectionHistoryDepth(int)),
                this, SLOT(setAngleDetectionHistoryDepth(int)));

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateWindowSize(size_t)),
                this, SLOT(setWindowSize(size_t)));

        connect(dynamic_cast<QObject*>(mSettingsProvider.get()), SIGNAL(updateVolume(double)),
                this, SLOT(setVolume(double)));
    }

    if (args.durationSetFlag()) {
        QTimer::singleShot(args.duration(), this, SLOT(finish()));
    }
}

void TrikSoundController::addAudioEventListener(const TrikSoundController::ListenerPtr& listener)
{
    mListeners.push_back(listener);
}

void TrikSoundController::bufferReadyReadHandler()
{
//    qDebug() << "readyRead!";
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
        assert(mAngleDetector != nullptr);
        event.setAngle(mAngleDetector->getAngle());
    }

    notify(event);
}

void TrikSoundController::handleSingleChannel()
{
    auto chlBegin = mWindowCopy.begin();
    auto chlEnd   = chlBegin + mWindowSize;
    mBufferAdapter->read((char*)mTmpWindowCopy.data(), mWindowSize * sizeof(sample_type));
//    copy(mBufferAdapter->readBegin(), mBufferAdapter->readEnd(), chlBegin);

    mFilter->handleWindow(chlBegin, chlEnd);
}

void TrikSoundController::handleDoubleChannel()
{
    auto chl1Begin = mWindowCopy.begin();
    auto chl1End   = chl1Begin + mWindowSize;
    auto chl2Begin = chl1End;
    auto chl2End   = mWindowCopy.end();

    mBufferAdapter->read((char*)mTmpWindowCopy.data(), CHANNEL_COUNT * mWindowSize * sizeof(sample_type));

    extractChannel<CHANNEL_COUNT, 0>(mTmpWindowCopy.begin(), mTmpWindowCopy.end(), chl1Begin);
    extractChannel<CHANNEL_COUNT, 1>(mTmpWindowCopy.begin(), mTmpWindowCopy.end(), chl2Begin);

    mStereoFilter->handleWindow(make_pair(chl1Begin, chl1End),
                                make_pair(chl2Begin, chl2End));
}

void TrikSoundController::notify(const AudioEvent& event)
{
//    qDebug() << "angle: " << event.angle();
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
    mBufferAdapter->clear();
    // in case of changes in windowSize
    mBuffer->resize(BUFFER_CAPACITY * mWindowSize);
    run();
}

void TrikSoundController::stop()
{
    mDeviceManager->stop();
}

void TrikSoundController::finish()
{
    stop();
    emit finished();
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


TrikSoundController::Settings::Settings():

    mSingleChannelFlag(false)
  , mFilteringFlag(false)
  , mAngleDetectionFlag(false)
  , mRecordStreamFlag(false)

  , mSampleRate(44100)
  , mSampleSize(16)
  , mSampleType(QAudioFormat::SignedInt)

  , mAngleDetectionHistoryDepth(5)
  , mWindowSize(1024)
  , mVolume(1.0)

  , mMicrDist(10.0)

  , mDurationSetFlag(false)
  , mDuration(0)
{}


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

bool TrikSoundController::Settings::recordStreamFlag() const
{
    return mRecordStreamFlag;
}

void TrikSoundController::Settings::setRecordStreamFlag(bool recordStreamFlag)
{
    mRecordStreamFlag = recordStreamFlag;
}

QString TrikSoundController::Settings::outputWavFilename() const
{
    return mOutputWavFilename;
}

void TrikSoundController::Settings::setOutputWavFilename(const QString& outputWavFilename)
{
    mOutputWavFilename = outputWavFilename;
}

int TrikSoundController::Settings::duration() const
{
    return mDuration;
}

void TrikSoundController::Settings::setDuration(int duration)
{
    mDuration = duration;
}

bool TrikSoundController::Settings::durationSetFlag() const
{
    return mDurationSetFlag;
}

void TrikSoundController::Settings::setDurationSetFlag(bool durationSetFlag)
{
    mDurationSetFlag = durationSetFlag;
}
