#include "audioCaptureFilter.h"

#include <limits>

#include <QDebug>

using namespace trikSound;

AudioCaptureFilter::AudioCaptureFilter(const QSharedPointer<AudioDeviceManager>& deviceManager,
                                       size_t frameLength,
                                       QObject* parent):
    QObject(parent)
  , mDeviceManager(deviceManager)
  , mBuffer(deviceManager->buffer())
  , mFrameLength(frameLength)
{
    size_t minCapacity = 4 * frameLength;
    if (mDeviceManager->bufferCapacity() < minCapacity) {
        qDebug() << "Resize AudioDeviceManager internal buffer to fit frameLength of AudioCaptureFilter. "
                 << "Old size: " << mDeviceManager->bufferCapacity() << "; "
                 << "New size: " << minCapacity << " bytes";
        mDeviceManager->setBufferCapacity(minCapacity);
    }

    connect(mBuffer.data(), SIGNAL(readyRead()), this, SLOT(bufferReadyReadHandler()));
}

AudioBuffer AudioCaptureFilter::input(const AudioBuffer& buf)
{
    Q_UNUSED(buf);
    return AudioBuffer(QByteArray(), QAudioFormat());
}

void AudioCaptureFilter::bufferReadyReadHandler()
{
//    qDebug() << "AudioCaptureFilter - bytes available: " << mBuffer->bytesAvailable();

    if (mBuffer->bytesAvailable() > mFrameLength) {
        emit output(AudioBuffer(mBuffer->read(mFrameLength), mDeviceManager->audioFormat()));
    }
}
