#include "include/internal/soundAngleSensor.h"

#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QStringList>

#include "include/internal/math_utils.h"
#include "include/internal/types.h"
#include "include/internal/wav_file.h"


#include "include/internal/wavfile.h"
#include "include/internal/filter.h"

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

void SoundAngleSensor::run()
{
    try {
        parseArgs();
    }
    catch (ArgumentsException& exc) {
        cout << "Arguments are incorrect. Error: " << exc.what() << endl;
        emit finished();
        return;
    }

    WavFile file(mFilename);
    if (!file.open(WavFile::ReadOnly)) {
        cout << "Cannot open file " << mFilename.toAscii().data() << endl;
        emit finished();
        return;
    }

    // offset 1000 samles
    const int offset = 1000;
    if (file.sampleCount() < offset) {
        cout << "File is too short" << endl;
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

    emit finished();
}

void SoundAngleSensor::parseArgs()
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
