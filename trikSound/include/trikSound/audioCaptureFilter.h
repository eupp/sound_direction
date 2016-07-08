/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QObject>
#include <QSharedPointer>

#include "triksound_global.h"
#include "audioFilter.h"
#include "audioDeviceManager.h"

namespace trikSound {

//class TRIKSOUNDSHARED_EXPORT AudioCaptureFilter: public QObject
//{
//    Q_OBJECT
//public:
//    AudioCaptureFilter(const std::shared_ptr<AudioDeviceManager>& deviceManager,
//                       size_t frameLength,
//                       QObject* parent = 0);

//signals:
//    void output(AudioBuffer buf);
//public slots:
//    AudioBuffer input(const AudioBuffer &buf);
//private slots:
//    void bufferReadyReadHandler();
//private:
//    std::shared_ptr<AudioDeviceManager> mDeviceManager;
//    std::shared_ptr<QIODevice> mBuffer;
//    quint16 mFrameLength;
//};

}
