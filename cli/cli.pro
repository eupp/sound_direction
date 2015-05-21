
include(../global.pri)

QT       += core multimedia
QT       += gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


TRIKRUNTIME_SOURCES_PATH = /home/evgeniy/projects/trik/trikRuntime/trikRuntime
TRIKRUNTIME_PATH = $$TRIKRUNTIME_SOURCES_PATH/bin/arm-release

INCLUDEPATH += \
            $$TRIKRUNTIME_SOURCES_PATH/trikControl/include/

LIBS += -L$$TRIKRUNTIME_PATH -ltrikControl -ltrikKernel -lqslog
QMAKE_LFLAGS += -Wl,-O1,-rpath,.

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

