#include "include/internal/audioCaptureFilter.h"

#include <limits>

#include <QDebug>


AudioCaptureFilter::AudioCaptureFilter(const QSharedPointer<AudioDeviceManager>& deviceManager,
                                       quint16 frameLength,
                                       QObject* parent):
    QObject(parent)
  , mDeviceManager(deviceManager)
  , mBuffer(deviceManager->buffer())
  , mFrameLength(frameLength)
{
    quint16 minCapacity = qMax((quint16)(4 * frameLength), std::numeric_limits<quint16>::max());
    if (mDeviceManager->bufferCapacity() < minCapacity) {
        qDebug() << "Resize AudioDeviceManager internal buffer to fit frameLength of AudioCaptureFilter. "
                 << "New size: " << minCapacity << " bytes";
        mDeviceManager->setBufferCapacity(minCapacity);
    }

    connect(mBuffer.data(), SIGNAL(readyRead()), this, SLOT(bufferReadyReadHandler()));
}

void AudioCaptureFilter::input(const AudioBuffer& buf)
{
    Q_UNUSED(buf);
}

void AudioCaptureFilter::bufferReadyReadHandler()
{
    qDebug() << "AudioCaptureFilter - bytes available: " << mBuffer->bytesAvailable();

    if (mBuffer->bytesAvailable() > mFrameLength) {
        emit output(AudioBuffer(mBuffer->read(mFrameLength), mDeviceManager->audioFormat()));
    }
}
