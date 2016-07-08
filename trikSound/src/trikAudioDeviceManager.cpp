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

#include "trikAudioDeviceManager.h"

#include <QProcess>

using namespace trikSound;

const int TrikAudioDeviceManager::mMaxVolume = 119;

TrikAudioDeviceManager::TrikAudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                                               const QAudioFormat& audioFormat,
                                               const std::shared_ptr<QIODevice>& buffer,
                                               bool forceInit):
    AudioDeviceManager(deviceInfo, audioFormat, buffer)
  , mVolume(0.5)
{

    if (forceInit) {
        QStringList initCommands;
        initCommands << "amixer set \"Right PGA Mixer Mic3R\" on"
                     << "amixer set \"Left PGA Mixer Mic3L\" on"
                     << "amixer set \"Right PGA Mixer Line2R\" on"
                     << "amixer set \"Left PGA Mixer Line2L\" on"
                     << "amixer set AGC on";

        for (auto& command: initCommands) {
            int res = QProcess::execute(command);
            if (res < 0) {
                throw InitException((command + " - command failed").toAscii().data());
            }
        }

    }
    setVolume(mVolume);
}

double TrikAudioDeviceManager::volume() const
{
    return mVolume;
}

void TrikAudioDeviceManager::setVolume(double vol)
{
    mVolume = vol;
    QString command = "amixer set PGA " + QString().setNum(static_cast<int>(vol * mMaxVolume));
    int res = QProcess::execute(command);
    if (res < 0) {
        throw CommandException((command + " - command failed").toAscii().data());
    }
}
