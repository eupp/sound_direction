#include "include/internal/trikSoundApplication.h"

#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QStringList>

#include "include/internal/types.h"
#include "include/internal/utils.h"
#include "include/internal/wavfile.h"
#include "include/internal/filter.h"
#include "include/internal/angleDetector.h"

using namespace std;
using namespace fpml;

class ArgumentsException: public exception
{
public:
    ArgumentsException(const char* msg):
        mMsg(msg)
    {}

    ~ArgumentsException() throw()
    {}

    const char* what() const throw()
    {
        return mMsg.toAscii().data();
    }
private:
    QString mMsg;
};

TrikSoundApplication::TrikSoundApplication(QObject* parent):
    QObject(parent)
  , out(stdout, QIODevice::WriteOnly)
{}

void TrikSoundApplication::run()
{
    try {
        parseArgs();
    }
    catch (ArgumentsException& exc) {
        out << "Arguments are incorrect. Error: " << exc.what() << endl;
        emit finished();
        return;
    }

    WavFile file(mFilename);
    if (!file.open(WavFile::ReadOnly)) {
        out << "Cannot open file " << mFilename.toAscii().data() << endl;
        emit finished();
        return;
    }

    // offset 1000 samles
    const int offset = 1000;
    if (file.sampleCount() < offset) {
        out << "File is too short" << endl;
        emit finished();
        return;
    }
    file.seek(offset);

    AudioBuffer buf = file.readAll();
    AudioBuffer chl1 = buf.leftChannel();
    AudioBuffer chl2 = buf.rightChannel();

    debug_print("ch1.test", (sample_t*) chl1.data(), chl1.sampleCount());
    debug_print("ch2.test", (sample_t*) chl2.data(), chl2.sampleCount());

    AudioFilter filter;
    AudioBuffer filt1 = filter.filter(chl1);
    AudioBuffer filt2 = filter.filter(chl2);

    debug_print("filt1.test", (sample_t*) filt1.data(), filt1.sampleCount());
    debug_print("filt2.test", (sample_t*) filt2.data(), filt2.sampleCount());

    AngleDetector detector;
    double angle = 0;
    try {
        angle = detector.getAngle(filt1, filt2, mMicrDist);
    }
    catch (AngleDetector::IncorrectSignals& exc) {
        out << "Internal error occurred" << endl;
        emit finished();
        return;
    }

    out << "Angle: " << angle << endl;

    emit finished();
}

void TrikSoundApplication::parseArgs()
{
    bool filenameSet = false;
    bool micrDistSet = false;

    QStringList argv = QCoreApplication::arguments();
    int argc = argv.size();

    if (argc <= 1) {
        throw ArgumentsException("To few arguments. Specify filename and distance between microphones");
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i] == QString("-f")) {
            if (++i >= argc) {
                throw ArgumentsException("Filename is missing");
            }
            mFilename = argv[i];
            filenameSet = true;
        }
        else if (argv[i] == QString("-c")) {
            if (++i >= argc) {
                throw ArgumentsException("Microphone distance is missing");
            }
            mMicrDist = argv[i].toDouble(&micrDistSet);
        }
    }
    if (!filenameSet) {
        throw ArgumentsException("Filename is missing");
    }
    if (!micrDistSet) {
        throw ArgumentsException("Microphone distance is missing or incorrect");
    }
}

void TrikSoundApplication::printAllDevicesInfo()
{
    out << "INPUT DEVICES:" << endl;
    for (auto& info: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        printDeviceInfo(info);
    }
    out << "OUTPUT DEVICES:" << endl;
    for (auto& info: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        printDeviceInfo(info);
    }
}

void TrikSoundApplication::printDeviceInfo(const QAudioDeviceInfo& info)
{
    out << "Device name: " << info.deviceName() << endl;
    QString indent = "*   ";
    out << indent << "Codecs: ";
    for (auto& codec: info.supportedCodecs()) {
        out << codec << " ";
    }
    out << endl;
    out << indent << "Sample rates: ";
    for (auto& rate: info.supportedSampleRates()) {
        out << rate << " ";
    }
    out << endl;
    out << indent << "Sample sizes: ";
    for (auto& ssize: info.supportedSampleSizes()) {
        out << ssize << " ";
    }
    out << endl;
    out << indent << "Sample types: ";
    for (auto& type: info.supportedSampleTypes()) {
        out << type << " ";
    }
    out << endl;
    out << indent << "Channels count: ";
    for (auto& count: info.supportedChannelCounts()) {
        out << count << " ";
    }
    out << endl;
    out << indent << "Byte orders: ";
    for (auto& order: info.supportedByteOrders()) {
        out << order << " ";
    }
    out << endl;
}
