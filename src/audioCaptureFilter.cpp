#include "include/internal/audioCaptureFilter.h"

#include <limits>

#include <QDebug>


AudioCaptureFilter::AudioCaptureFilter(const QSharedPointer<AudioDeviceManager>& deviceManager, quint16 frameLength):
    mDeviceManager(deviceManager)
  , mFrameLength(frameLength)
{
    quint16 minCapacity = qMax(4 * frameLength, std::numeric_limits<quint16>::max());
    if (mDeviceManager->bufferCapacity() < minCapacity) {
        qDebug() << "Resize AudioDeviceManager internal buffer to fit frameLength of AudioCaptureFilter. "
                 << "New size: " << minCapacity << " bytes";
        mDeviceManager->setBufferCapacity(minCapacity);
    }
}
