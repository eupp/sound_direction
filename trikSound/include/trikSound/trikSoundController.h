#pragma once

#include <memory>

#include <QObject>

#include "triksound_global.h"

#include "circularBuffer.h"
#include "circularBufferQAdapter.h"
#include "audioFilter.h"
#include "stereoAudioFilter.h"
#include "audioDeviceManager.h"
#include "angleDetector.h"
#include "iAudioEventListener.h"
#include "iSettingsProvider.h"
#include "audioPipe.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikSoundController : public QObject
{
    Q_OBJECT
public:

    typedef std::shared_ptr<ISettingsProvider> SettingsProviderPtr;
    typedef std::shared_ptr<IAudioEventListener> ListenerPtr;

    // settings for controller constructor

    class Settings
    {
    public:

        // construct Setting object with default controller settings

        Settings();

        int angleDetectionHistoryDepth() const;
        void setAngleDetectionHistoryDepth(int angleDetectionHistoryDepth);

        size_t windowSize() const;
        void setWindowSize(const size_t& windowSize);

        double volume() const;
        void setVolume(double volume);

        bool singleChannelFlag() const;
        void setSingleChannelFlag(bool singleChannelFlag);

        bool filteringFlag() const;
        void setFilteringFlag(bool filteringFlag);

        bool angleDetectionFlag() const;
        void setAngleDetectionFlag(bool angleDetectionFlag);

        int sampleRate() const;
        void setSampleRate(int sampleRate);

        int sampleSize() const;
        void setSampleSize(int sampleSize);

        QAudioFormat::SampleType sampleType() const;
        void setSampleType(const QAudioFormat::SampleType& sampleType);

        double micrDist() const;
        void setMicrDist(double micrDist);

        bool recordStreamFlag() const;
        void setRecordStreamFlag(bool recordStreamFlag);

        QString outputWavFilename() const;
        void setOutputWavFilename(const QString& outputWavFilename);

        int duration() const;
        void setDuration(int duration);

        bool durationFlag() const;
        void setDurationFlag(bool durationFlag);

    private:

        // flags

        bool mSingleChannelFlag;
        bool mFilteringFlag;
        bool mAngleDetectionFlag;
        bool mRecordStreamFlag;

        // audio format parameters

        int mSampleRate;
        int mSampleSize;
        QAudioFormat::SampleType mSampleType;

        // controller settings

        int mAngleDetectionHistoryDepth;
        size_t mWindowSize;
        double mVolume;

        // angle detector arguments

        double mMicrDist;

        // duration settings

        int mDuration;
        bool mDurationFlag;

        // other settings

        QString mOutputWavFilename;
    };

    // exception during initialization

    class InitException : public TrikSoundException
    {
    public:
        InitException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    // constructor

    TrikSoundController(const Settings& args, const SettingsProviderPtr& provider = SettingsProviderPtr());

    // add listeners

    void addAudioEventListener(const ListenerPtr& listener);

    // controller runtime settings

    int angleDetectionHistoryDepth() const;
    size_t windowSize() const;
    double volume() const;

    // controller static settings

    bool singleChannelFlag() const;

public slots:

    // manage controller loop

    void run();
    void restart();
    void stop();

    void finish();

    // controller runtime settings

    void setAngleDetectionHistoryDepth(int historyDepth);
    void setWindowSize(size_t size);
    void setVolume(double vol);

signals:

    void finished();

private slots:

    void bufferReadyReadHandler();

private:

    typedef sample_type                 range_value_type;
    typedef std::vector<sample_type>    WindowContainer;
    typedef WindowContainer::iterator   BufferIterator;

    typedef std::shared_ptr<CircularBuffer>                             CircularBufferPtr;
    typedef std::shared_ptr<CircularBufferQAdapter>                     CircularBufferQAdapterPtr;
    typedef std::shared_ptr<AngleDetector<BufferIterator>>              AngleDetectorPtr;
    typedef std::unique_ptr<AudioDeviceManager>                         AudioDeviceManagerPtr;
    typedef AudioFilter<BufferIterator>::FilterPtr                      FilterPtr;
    typedef StereoAudioFilter<BufferIterator>::FilterPtr                StereoFilterPtr;

    void handleSingleChannel();
    void handleDoubleChannel();
    void notify(const AudioEvent& event);

    // maximum available count of channels
    static const int CHANNEL_COUNT = 2;
    // buffer capacity in terms of count of windows it stores
    static const int BUFFER_CAPACITY = 20;

    // circular buffer

    CircularBufferPtr mBuffer;
    CircularBufferQAdapterPtr mBufferAdapter;

    // size of window in samples

    size_t mWindowSize;

    // copy of processing window

    WindowContainer mWindowCopy;

    // audio device management

    AudioDeviceManagerPtr mDeviceManager;

    // filters

    StereoAudioPipe<BufferIterator> mPipe;
    AngleDetectorPtr mAngleDetector;

    // settings provider

    SettingsProviderPtr mSettingsProvider;

    // listeners

    std::vector<ListenerPtr> mListeners;

    // flags

    bool mAngleDetectionFlag;
    bool mSingleChannelFlag;
};

}
