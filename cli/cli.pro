 # Copyright 2014 - 2016 Evgenii Moiseenko.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.


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

#LIBS += -L$$TRIKRUNTIME_PATH -ltrikControl -ltrikKernel -lqslog
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

