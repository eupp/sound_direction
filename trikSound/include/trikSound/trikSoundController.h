#pragma once

#include <memory>

#include <QObject>

#include "triksound_global.h"
#include "circularBufferQAdapter.h"
#include "audioFilter.h"
#include "stereoAudioFilter.h"
#include "audioDeviceManager.h"
#include "angleDetector.h"
#include "audioEventListener.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikSoundController : public QObject
{
    Q_OBJECT
public:

    TrikSoundController();

public slots:

    void setAngleDetectionHistoryDepth(int historyDepth);
    void setFrameLength(size_t length);
    void setVolume(double vol);


signals:

private slots:

    void bufferReadyReadHandler();

private:

    typedef sample_type range_value_type;
    typedef std::vector<sample_type> WindowContainer;
    typedef WindowContainer::iterator BufferIterator;


    typedef std::shared_ptr<AngleDetector<BufferIterator>> AngleDetectorPtr;
    typedef std::shared_ptr<AudioEventListener> ListenerPtr;
    typedef std::unique_ptr<AudioDeviceManager> AudioDeviceManagerPtr;


    void notify(const AudioEvent& event);

    static const int CHANNEL_COUNT = 2;

    AudioDeviceManagerPtr mDeviceManager;
    CircularBufferQAdapter::CircularBufferPtr mBuffer;
    CircularBufferQAdapter mBufferAdapter;
    StereoAudioFilter<BufferIterator>::FilterPtr mFilter;
    AngleDetectorPtr mAngleDetector;

    std::vector<ListenerPtr> mListeners;

    size_t mFrameLength;

    std::vector<range_value_type> mWindowCopy;

    bool mAngleDetectionFlag;
};

}
