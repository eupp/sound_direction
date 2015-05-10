
include(../global.pri)

QT       += core multimedia
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/argumentParser.cpp \
    src/controlFifo.cpp \
    src/outputFifo.cpp \
    src/settings.cpp

HEADERS += \
    include/internal/argumentParser.h \
    include/internal/controlFifo.h \
    include/internal/utils.h \
    include/internal/outputFifo.h \
    include/internal/settings.h

uses(trikSound)

