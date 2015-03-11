#include "trikAudioDeviceManager.h"

#include <QProcess>

using namespace trikSound;

const int TrikAudioDeviceManager::mMaxVolume = 119;

TrikAudioDeviceManager::TrikAudioDeviceManager(const QAudioDeviceInfo& deviceInfo,
                                               const QAudioFormat& audioFormat,
                                               const std::shared_ptr<QIODevice>& buffer):
    AudioDeviceManager(deviceInfo, audioFormat, buffer)
  , mVolume(1.0)
{
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
