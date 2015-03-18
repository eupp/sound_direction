
include(../global.pri)

QT       += multimedia
QT       -= gui

TEMPLATE = lib

DEFINES += TRIKSOUND_LIBRARY

INCLUDEPATH += include/fpml

SOURCES += \
    src/audioBuffer.cpp \
    src/audioCaptureFilter.cpp \
    src/audioDeviceManager.cpp \
#    src/thresholdVadFilter.cpp \
    src/trikAudioDeviceManager.cpp \
    src/circularBufferQAdapter.cpp \
    src/trikSoundController.cpp \
    src/wavFile.cpp

HEADERS += \
    triksound_global.h \
    include/fpml/fixed_point.h \
    include/internal/angleDetectorImpl.h \
    include/internal/circularBuffer.h \
    include/internal/debugUtils.h \
    include/internal/digitalAudioFilterImpl.h \
    include/internal/realTypeTraits.h \
    include/internal/utils.h \
    include/trikSound/angleDetector.h \
    include/trikSound/audioBuffer.h \
#    include/trikSound/audioCaptureFilter.h \
    include/trikSound/audioDeviceManager.h \
    include/trikSound/digitalAudioFilter.h \
#    include/trikSound/thresholdVadFilter.h \
    include/trikSound/trikAudioDeviceManager.h \
    include/trikSound/trikSoundException.h \
    include/trikSound/types.h \
    include/trikSound/triksound_global.h \
    include/trikSound/debugUtils.h \
    include/trikSound/utils.h \
    include/trikSound/circularBufferQAdapter.h \
    include/trikSound/audioFilter.h \
    include/trikSound/splitFilter.h \
    include/trikSound/stereoAudioFilter.h \
    include/trikSound/trikSoundController.h \
    include/trikSound/audioEvent.h \
    include/trikSound/wavFile.h \
    include/trikSound/emptyFilter.h \
    include/trikSound/emptyStereoFilter.h \
    include/trikSound/iAudioEventListener.h

