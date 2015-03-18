#pragma once

#include <memory>

#include <QObject>

#include "triksound_global.h"
#include "circularBufferQAdapter.h"
#include "audioFilter.h"
#include "stereoAudioFilter.h"
#include "audioDeviceManager.h"
#include "angleDetector.h"
#include "iAudioEventListener.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikSoundController : public QObject
{
    Q_OBJECT
public:

    typedef std::shared_ptr<IAudioEventListener> ListenerPtr;

    class Settings
    {
    public:

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

    private:

        // flags

        bool mSingleChannelFlag;
        bool mFilteringFlag;
        bool mAngleDetectionFlag;

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
    };

    class InitException : public TrikSoundException
    {
    public:
        InitException(const char* msg):
            TrikSoundException(msg)
        {}
    };

    TrikSoundController(const Settings& args);

    void addAudioEventListener(const ListenerPtr& listener);

    int angleDetectionHistoryDepth() const;
    size_t windowSize() const;
    double volume() const;

    bool singleChannelFlag() const;

public slots:

    void setAngleDetectionHistoryDepth(int historyDepth);
    void setWindowSize(size_t size);
    void setVolume(double vol);

signals:

private slots:

    void bufferReadyReadHandler();

private:

    typedef sample_type range_value_type;
    typedef std::vector<sample_type> WindowContainer;
    typedef WindowContainer::iterator BufferIterator;


    typedef std::shared_ptr<CircularBufferQAdapter::CircularBuffer> CircularBufferPtr;
    typedef std::shared_ptr<CircularBufferQAdapter> CircularBufferQAdapterPtr;
    typedef std::shared_ptr<AngleDetector<BufferIterator>> AngleDetectorPtr;
    typedef std::unique_ptr<AudioDeviceManager> AudioDeviceManagerPtr;
    typedef AudioFilter<BufferIterator>::FilterPtr FilterPtr;
    typedef StereoAudioFilter<BufferIterator>::FilterPtr StereoFilterPtr;

    void handleSingleChannel();
    void handleDoubleChannel();
    void notify(const AudioEvent& event);

    static const int CHANNEL_COUNT = 2;
    // buffer capacity in terms of count of windows it stores
    static const int BUFFER_CAPACITY = 10;

    CircularBufferPtr mBuffer;
    CircularBufferQAdapterPtr mBufferAdapter;
    // size of window in samples
    size_t mWindowSize;
    WindowContainer mWindowCopy;

    AudioDeviceManagerPtr mDeviceManager;

    AudioFilter<BufferIterator>::FilterPtr mFilter;
    StereoAudioFilter<BufferIterator>::FilterPtr mStereoFilter;
    AngleDetectorPtr mAngleDetector;

    std::vector<ListenerPtr> mListeners;

    bool mAngleDetectionFlag;
    bool mSingleChannelFlag;
};

}
