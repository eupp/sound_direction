#include "trikSoundController.h"

#include <vector>

#include <QDebug>
#include <QTimer>

#include "utils.h"

#include "singleChannelCircularBuffer.h"
#include "doubleChannelCircularBuffer.h"

#include "digitalAudioFilter.h"
#include "splitFilter.h"
#include "recordFilter.h"
#include "stereoRecordFilter.h"

#include "fileAudioStream.h"
#include "captureAudioStream.h"

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
    mWindowSize(args.windowSize())
  , mWindowCopy(CHANNEL_COUNT * args.windowSize())
  , mAngleDetectionFlag(args.angleDetectionFlag())
  , mSingleChannelFlag(args.singleChannelFlag())

  , mSettingsProvider(provider)

{
    if (args.singleChannelFlag()) {
        mBuffer = make_shared<SingleChannelCircularBuffer>(BUFFER_CAPACITY * args.windowSize());
    }
    else {
        mBuffer = make_shared<DoubleChannelCircularBuffer>(BUFFER_CAPACITY * args.windowSize());
    }
    mBufferAdapter = make_shared<CircularBufferQAdapter>(mBuffer);
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

    if (args.fileInputFlag()) {
        mAudioStream.reset(new FileAudioStream(args.inputWavFilename()));
    }
    else {
        mAudioStream.reset(new CaptureAudioStream(mDeviceManager, mBufferAdapter));
    }

    auto monoPipe = make_shared<AudioPipe<BufferIterator>>();
    StereoFilterPtr split = make_shared<SplitFilter<BufferIterator>>(monoPipe);
    mPipe.insertFilter(mPipe.end(), split);

    if (args.filteringFlag()) {
        FilterPtr filter = make_shared<DigitalAudioFilter<BufferIterator>>();
        monoPipe->insertFilter(monoPipe->end(), filter);
    }

    if (args.angleDetectionFlag()) {
        if (args.singleChannelFlag()) {
            throw InitException("TrikSoundController error."
                                "Angle detection enabled with single audio channel");
        }

        mAngleDetector = make_shared<AngleDetector<BufferIterator>>(args.sampleRate(),
                                                                    args.micrDist(),
                                                                    args.angleDetectionHistoryDepth());
        mPipe.insertFilter(mPipe.end(), static_pointer_cast<StereoAudioFilter<BufferIterator>>(mAngleDetector));
    }

    if (args.recordStreamFlag()) {
        RecordFilter<BufferIterator>::WavFilePtr wavFile = make_shared<WavFile>(args.outputWavFilename());
        wavFile->open(WavFile::WriteOnly, fmt);
        if (args.singleChannelFlag()) {
            FilterPtr record = make_shared<RecordFilter<BufferIterator>>(wavFile);
            monoPipe->insertFilter(monoPipe->end(), record);
        }
        else {
            StereoFilterPtr record = make_shared<StereoRecordFilter<BufferIterator>>(wavFile);
            mPipe.insertFilter(mPipe.end(), record);
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
    mAudioStream->read(mWindowCopy.data(), mWindowSize);
    auto begin = mWindowCopy.begin();
    auto end   = begin + mWindowSize;

    mPipe.handleWindow(make_pair(begin, end),
                       StereoAudioFilter<BufferIterator>::make_empty_range());
}

void TrikSoundController::handleDoubleChannel()
{
    mAudioStream->read(mWindowCopy.data(), 2 * mWindowSize);
    auto leftBegin  = mWindowCopy.begin();
    auto leftEnd    = leftBegin + mWindowSize;
    auto rightBegin = leftEnd;
    auto rightEnd   = mWindowCopy.end();

    mPipe.handleWindow(make_pair(leftBegin , leftEnd),
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
    mAudioStream->run();
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
    mAudioStream->stop();
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


TrikSoundController::Settings::Settings():

    mSingleChannelFlag(false)
  , mFilteringFlag(false)
  , mAngleDetectionFlag(false)
  , mRecordStreamFlag(false)
  , mFileInputFlag(false)

  , mSampleRate(44100)
  , mSampleSize(16)
  , mSampleType(QAudioFormat::SignedInt)

  , mAngleDetectionHistoryDepth(5)
  , mWindowSize(1024)
  , mVolume(1.0)

  , mMicrDist(10.0)

  , mDurationFlag(false)
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

bool TrikSoundController::Settings::durationFlag() const
{
    return mDurationFlag;
}

void TrikSoundController::Settings::setDurationFlag(bool durationSetFlag)
{
    mDurationFlag = durationSetFlag;
}

QString TrikSoundController::Settings::inputWavFilename() const
{
    return mInputWavFilename;
}

void TrikSoundController::Settings::setInputWavFilename(const QString& inputWavFilename)
{
    mInputWavFilename = inputWavFilename;
}

bool TrikSoundController::Settings::fileInputFlag() const
{
    return mFileInputFlag;
}

void TrikSoundController::Settings::setFileInputFlag(bool fileInputFlag)
{
    mFileInputFlag = fileInputFlag;
}

