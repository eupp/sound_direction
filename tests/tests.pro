
include(../global.pri)

QT       += core multimedia
QT       -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    testCircularBuffer.cpp

LIBS+= -lboost_unit_test_framework

uses(trikSound)
