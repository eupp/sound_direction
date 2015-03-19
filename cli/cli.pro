
include(../global.pri)

QT       += core multimedia gui
#QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/argumentParser.cpp \
    src/mainWindow.cpp

HEADERS += \
    include/internal/argumentParser.h \
    include/internal/mainWindow.h

FORMS += \
    ui/mainWindow.ui

uses(trikSound)

