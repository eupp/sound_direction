TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

QT += core multimedia
QT -= gui

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
    src/wav_file.cpp \
    src/wavfile.cpp \
    src/audioBuffer.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/internal/wav_file.h \
    include/internal/math_utils.h \
    include/internal/types.h \
    include/fpml/fixed_point.h \
    include/internal/wavfile.h \
    include/internal/audioBuffer.h

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
