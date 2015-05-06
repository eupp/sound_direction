#include "controlFifo.h"

#include <algorithm>
#include <sstream>

#include <QTextStream>
#include <QDebug>

#include "utils.h"

using namespace std;

ControlFifo::ControlFifo():
    mFile(stdin)
  , mNotifier(fileno(mFile), QSocketNotifier::Read)
{
    fill(mCmdBuffer, end(mCmdBuffer), 0);
    connect(&mNotifier, SIGNAL(activated(int)), this, SLOT(readData()));
    mNotifier.setEnabled(true);
}

void ControlFifo::readData()
{
    mNotifier.setEnabled(false);
    while (!fgets(mCmdBuffer, CMD_BUFFER_SIZE, mFile)) {
        readCommand();
        fill(mCmdBuffer, end(mCmdBuffer), 0);
    }
    mNotifier.setEnabled(true);
}

void ControlFifo::readCommand()
{
    QString qstr(mCmdBuffer);
    QTextStream in(&qstr, QIODevice::ReadOnly);

    QString cmd;
    in >> cmd;
    if (cmd == "set") {
        QString paramName;
        QString param;
        in >> paramName >> param;

        if (in.status() != QTextStream::Ok) {
            qDebug() << "ControlFifo error. Unrecognized command: " << qstr;
            return;
        }

        if (paramName == "angleHistoryDepth") {
            int historyDepth = convertParam<int>(paramName.toAscii().data(), param,
                                                 "ControlFifo error. Cannot convert historyDepth to int."
                                                 "Command: " + qstr.toStdString());
            emit updateAngleDetectionHistoryDepth(historyDepth);
        }
        else if (paramName == "windowSize") {
            size_t windowSize = convertParam<size_t>(paramName.toAscii().data(), param,
                                          "ControlFifo error. Cannot convert windowSize to size_t."
                                          "Command: " + qstr.toStdString());
            emit updateWindowSize(windowSize);
        }
        else if (paramName == "volume") {
            double volume = convertParam<double>(paramName.toAscii().data(), param,
                                                 "ControlFifo error. Cannot convert volume to double."
                                                 "Command: " + qstr.toStdString());
            emit updateVolume(volume);
        }
    }
}
