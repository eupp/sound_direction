QT += core multimedia
QT -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle


trik {
	message("Building for Trik")
	DEFINES += TRIK
	BUILD_FOLDER = ../trik-build
	DESTDIR = $$BUILD_FOLDER/
	OBJECTS_DIR = $$BUILD_FOLDER/
	MOC_DIR = $$BUILD_FOLDER/
	RCC_DIR = $$BUILD_FOLDER/
	UI_DIR = $$BUILD_FOLDER/
        CONFIG -= debug
        CONFIG += release
}

SOURCES += \
    src/main.cpp \
    src/wavfile.cpp \
    src/audioBuffer.cpp \
    src/angleDetector.cpp \
    src/trikSoundApplication.cpp \
    src/audioDeviceManager.cpp \
    src/trikAudioDeviceManager.cpp \
    src/circularBuffer.cpp \
    src/audioCaptureFilter.cpp \
    src/thresholdVadFilter.cpp \
    src/digitalAudioFilter.cpp \
    src/angleFilter.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/internal/types.h \
    include/fpml/fixed_point.h \
    include/internal/wavfile.h \
    include/internal/audioBuffer.h \
    include/internal/realTypeTraits.h \
    include/internal/angleDetector.h \
    include/internal/utils.h \
    include/internal/trikSoundApplication.h \
    include/internal/audioDeviceManager.h \
    include/internal/trikAudioDeviceManager.h \
    include/internal/circularBuffer.h \
    include/internal/trikSoundException.h \
    include/internal/audioCaptureFilter.h \
    include/internal/iAudioFilter.h \
    include/internal/audioPipe.h \
    include/internal/thresholdVadFilter.h \
    include/internal/digitalAudioFilter.h \
    include/internal/angleFilter.h

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
