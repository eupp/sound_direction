
include(../global.pri)

QT       += core multimedia
QT       -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle


SOURCES += main.cpp \
    benchmarks.cpp

LIBS+= -lboost_system

HEADERS += \
    performanceTimer.h \
    benchmarks.h


uses(trikSound)
