
include(../global.pri)

QT       += core multimedia
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/trikSoundApplication.cpp \
    src/benchmark.cpp

HEADERS += \
    include/internal/trikSoundApplication.h \
    include/internal/benchmark.h

uses(trikSound)
