#include <QCoreApplication>
#include <QTimer>

#include "include/internal/trikSoundApplication.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    TrikSoundApplication* sensor = new TrikSoundApplication(&app);

    QObject::connect(sensor, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, sensor, SLOT(run()));

    return app.exec();
}

