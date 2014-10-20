TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/wav_file.cpp \
    src/math_utils.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/internal/wav_file.h \
    include/internal/math_utils.h \
    include/internal/types.h

