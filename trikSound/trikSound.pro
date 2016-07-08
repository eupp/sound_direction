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

QT       += multimedia
QT       -= gui

TEMPLATE = lib

DEFINES += TRIKSOUND_LIBRARY

INCLUDEPATH += include/fpml

SOURCES += \
    src/audioBuffer.cpp \
    src/audioDeviceManager.cpp \
    src/trikAudioDeviceManager.cpp \
    src/circularBufferQAdapter.cpp \
    src/trikSoundController.cpp \
    src/wavFile.cpp \
    src/singleChannelCircularBuffer.cpp \
    src/doubleChannelCircularBuffer.cpp \
    src/captureAudioStream.cpp \
    src/fileAudioStream.cpp \
    src/settings.cpp

HEADERS += \
    include/fpml/fixed_point.h \
    include/internal/angleDetectorImpl.h \
    include/internal/digitalAudioFilterImpl.h \
    include/internal/realTypeTraits.h \
    include/trikSound/angleDetector.h \
    include/trikSound/audioBuffer.h \
    include/trikSound/audioDeviceManager.h \
    include/trikSound/digitalAudioFilter.h \
    include/trikSound/trikAudioDeviceManager.h \
    include/trikSound/trikSoundException.h \
    include/trikSound/types.h \
    include/trikSound/triksound_global.h \
    include/trikSound/debugUtils.h \
    include/trikSound/utils.h \
    include/trikSound/circularBufferQAdapter.h \
    include/trikSound/audioFilter.h \
    include/trikSound/splitFilter.h \
    include/trikSound/stereoAudioFilter.h \
    include/trikSound/trikSoundController.h \
    include/trikSound/audioEvent.h \
    include/trikSound/wavFile.h \
    include/trikSound/iAudioEventListener.h \
    include/trikSound/iSettingsProvider.h \
    include/trikSound/stereoRecordFilter.h \
    include/trikSound/recordFilter.h \
    include/trikSound/audioPipe.h \
    include/trikSound/circularBuffer.h \
    include/trikSound/doubleChannelCircularBuffer.h \
    include/trikSound/singleChannelCircularBuffer.h \
    include/trikSound/audioStream.h \
    include/trikSound/captureAudioStream.h \
    include/trikSound/fileAudioStream.h \
    include/trikSound/initializer.h \
    include/trikSound/settings.h \
    include/trikSound/vadFilter.h \
    include/internal/vadFilterImpl.h \
    include/trikSound/stereoVadFilter.h \
    include/trikSound/vadFilterWrapper.h

