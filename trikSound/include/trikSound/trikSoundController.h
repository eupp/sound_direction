#pragma once

#include <memory>

#include <QObject>

#include "triksound_global.h"

#include "circularBufferQAdapter.h"
#include "audioDeviceManager.h"
#include "angleDetector.h"
#include "iAudioEventListener.h"
#include "iSettingsProvider.h"
#include "audioPipe.h"
#include "audioStream.h"
#include "settings.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikSoundController : public QObject
{
    Q_OBJECT
public:

    typedef std::shared_ptr<ISettingsProvider> SettingsProviderPtr;
    typedef std::shared_ptr<IAudioEventListener> ListenerPtr;

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
    typedef AudioFilter<BufferIterator>::FilterPtr                      FilterPtr;
    typedef StereoAudioFilter<BufferIterator>::FilterPtr                StereoFilterPtr;
    typedef std::shared_ptr<StereoAudioPipe<BufferIterator>>            StereoAudioPipePtr;

    typedef std::shared_ptr<AudioDeviceManager>                         AudioDeviceManagerPtr;
    typedef std::shared_ptr<AudioStream>                                AudioStreamPtr;

    void handleSingleChannel();
    void handleDoubleChannel();
    void notify(const AudioEvent& event);

    // maximum available count of channels
    static const int CHANNEL_COUNT = 2;

    // audio stream

    AudioStreamPtr mAudioStream;

    // circular buffer

    CircularBufferQAdapterPtr mBufferAdapter;

    // size of window in samples

    size_t mWindowSize;

    // copy of processing window

    WindowContainer mWindowCopy;

    // audio device management

    AudioDeviceManagerPtr mDeviceManager;

    // filters

    StereoAudioPipePtr mPipe;
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
