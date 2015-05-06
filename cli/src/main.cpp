#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

#include "trikSound/trikSoundController.h"

#include "argumentParser.h"
#include "outputFifo.h"
#include "controlFifo.h"

using namespace std;
using namespace trikSound;

int main(int argc, char** argv)
{
    try {
        QCoreApplication app(argc, argv);

        Settings settings = ArgumentParser::parse();
        shared_ptr<ISettingsProvider> provider = make_shared<ControlFifo>();
        shared_ptr<IAudioEventListener> eventListener = make_shared<OutputFifo>();

        TrikSoundController* controller = new TrikSoundController(settings, provider, &app);
        controller->addAudioEventListener(eventListener);

        QTimer::singleShot(0, controller, SLOT(run()));
        QObject::connect(controller, SIGNAL(finished()), &app, SLOT(quit()));

        return app.exec();
    }
    catch (exception& exc) {
        qDebug() << exc.what();
        return 0;
    }
}

