#include <QCoreApplication>
#include <QTimer>

#include "include/internal/soundAngleSensor.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    SoundAngleSensor* sensor = new SoundAngleSensor(&app);

    QObject::connect(sensor, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, sensor, SLOT(run()));

    return app.exec();
}

