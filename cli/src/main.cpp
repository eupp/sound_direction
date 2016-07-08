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

#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <QDebug>

#include "trikSound/trikSoundController.h"

#include "argumentParser.h"
#include "outputFifo.h"
#include "controlFifo.h"

using namespace std;

using trikSound::IAudioEventListener;
using trikSound::ISettingsProvider;
using trikSound::TrikSoundController;

int main(int argc, char** argv)
{
    try {
        QApplication app(argc, argv);

        Settings settings = ArgumentParser::parse();

        shared_ptr<ISettingsProvider> provider = make_shared<ControlFifo>();
        shared_ptr<IAudioEventListener> eventListener = make_shared<OutputFifo>(settings.viewSettings());

        TrikSoundController* controller = new TrikSoundController(settings.controllerSettings(),
                                                                  provider,
                                                                  &app);
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

