#include <QApplication>
#include <QTimer>
#include <QDebug>

#include "trikSound/trikSoundController.h"

#include "argumentParser.h"
#include "mainWindow.h"

using namespace std;
using namespace trikSound;

int main(int argc, char** argv)
{
    try {
        QApplication app(argc, argv);

        TrikSoundController::Settings settings = ArgumentParser::parse();
        shared_ptr<TrikSoundController> controller = make_shared<TrikSoundController>(settings);
        shared_ptr<MainWindow> window = make_shared<MainWindow>(controller);
        controller->addAudioEventListener(window);

        QTimer::singleShot(0, controller.get(), SLOT(run()));
        window->show();

        return app.exec();
    }
    catch (exception& exc) {
        qDebug() << exc.what();
        return 0;
    }
}

